#include "ata.h"
#include "../../libs/device.h"
#include "../../api/kernel_functions.h"

#include "../../libs/io.h"
#include "../../libs/types.h"
#include "../../libs/ata.h"
#include "../../libs/asm.h"

#define ATA_BASE 1

#define ATA_MASTER 0xA0
#define ATA_SLAVE 0xB0

// Status

#define ATA_SR_BSY     0x80    // Busy
#define ATA_SR_DRDY    0x40    // Drive ready
#define ATA_SR_DF      0x20    // Drive write fault
#define ATA_SR_DSC     0x10    // Drive seek complete
#define ATA_SR_DRQ     0x08    // Data request ready
#define ATA_SR_CORR    0x04    // Corrected data
#define ATA_SR_IDX     0x02    // Index
#define ATA_SR_ERR     0x01    // Error

// Errors

#define ATA_ER_BBK      0x80    // Bad block
#define ATA_ER_UNC      0x40    // Uncorrectable data
#define ATA_ER_MC       0x20    // Media changed
#define ATA_ER_IDNF     0x10    // ID mark not found
#define ATA_ER_MCR      0x08    // Media change request
#define ATA_ER_ABRT     0x04    // Command aborted
#define ATA_ER_TK0NF    0x02    // Track 0 not found
#define ATA_ER_AMNF     0x01    // No address mark

// Commands

#define ATA_CMD_READ_PIO          0x20
#define ATA_CMD_READ_PIO_EXT      0x24
#define ATA_CMD_READ_DMA          0xC8
#define ATA_CMD_READ_DMA_EXT      0x25
#define ATA_CMD_WRITE_PIO         0x30
#define ATA_CMD_WRITE_PIO_EXT     0x34
#define ATA_CMD_WRITE_DMA         0xCA
#define ATA_CMD_WRITE_DMA_EXT     0x35
#define ATA_CMD_CACHE_FLUSH       0xE7
#define ATA_CMD_CACHE_FLUSH_EXT   0xEA
#define ATA_CMD_PACKET            0xA0
#define ATA_CMD_IDENTIFY_PACKET   0xA1
#define ATA_CMD_IDENTIFY          0xEC

// Регистры IDE, смещаемые от base

#define REG_DATA        0   // данные (16 бит)
#define REG_ERROR       1   // чтение: Error, запись: Features
#define REG_FEATURES    1   // запись: Features (то же место, что Error при чтении)
#define REG_SECTOR_COUNT 2
#define REG_LBA_LOW     3
#define REG_LBA_MID     4
#define REG_LBA_HIGH    5
#define REG_DEVICE      6   // управление: Device/Head
#define REG_STATUS      7   // чтение: Status, запись: Command
#define REG_COMMAND     7   // запись: Command (то же место, что Status при чтении)


void* ata_funcs[] = {

};


// стандартная задержка для ATA
void ata_delay(unsigned int base){
    for (int i = 0; i < 5; i++)
        inb(base + 7);
}

// проверяет Занято ли устройство
int ata_wait_bsy(unsigned int base) {
    int timeout = 1000000;
    while (timeout-- > 0) {
        unsigned char status = inb(base + REG_STATUS);
        if ((status & ATA_SR_BSY) == 0) return 0; // ok
        in_out_wait();
    }
    return -1; // Timeout
}

int ata_wait_drq(unsigned int base) {
    int timeout = 1000000;
    while (timeout-- > 0) {
        unsigned char status = inb(base + REG_STATUS);
        if (ata_get_err(base) != 0) return -2; // err
        if (status & ATA_SR_DRQ) return 0; // ok
        in_out_wait();
    }
    return -1; // Timeout
}

int ata_get_err(unsigned int base){
    unsigned char status = inb(base + REG_STATUS);
    if (status & 0x01) return -1; // ERR
    if (status & 0x20) return -1; // DF
    else return 0; // ok
}

// Читаем данные с диска
// Читаем 512 байт (один сектор)
void ata_read(unsigned int base, unsigned char* buffer){
    for (int i = 0; i < 256; i++) {
        unsigned short word = inw(base);
        buffer[i*2] = word & 0xFF;
        buffer[i*2+1] = (word >> 8) & 0xFF;
    }
}

// Записываем данные на диск
// Записываем 512 байт (один сектор)
void ata_write(unsigned int base, unsigned short* src){
    for (int i = 0; i < 256; i++) {
        unsigned short word = src[i];
        outw(ATA_BASE, word);
    }
}

/*

// Чтение определенного секотра
// lba - номер сектора
// buffer - буффер
int ata_read_sector(unsigned int lba, unsigned char* buffer) {

    // Проверяем и ждем готовность диска
    if (ata_wait_ready() < 0) return -1;

    // Выбираем диск Master + LBA (старший байт)
    outb(ATA_BASE + 6, 0xE0 | ((lba >> 24) & 0x0F));

    // Параметры
    outb(ATA_BASE + 2, 1);              // кол-во секторов: 1
    outb(ATA_BASE + 3, lba & 0xFF);     // LBA 0-7
    outb(ATA_BASE + 4, (lba >> 8) & 0xFF);  // LBA 8-15
    outb(ATA_BASE + 5, (lba >> 16) & 0xFF); // LBA 16-23

    // Команда чтения
    outb(ATA_BASE + 7, 0x20);

    // Проверяем и ждем готовность диска
    if (ata_wait_ready() < 0) return -1;

    // Читаем 512 байт (один сектор)
    unsigned short temp[256];
    ata_read(temp);

    // переводим из массива short
    for (int i = 0; i < 256; i++) {
        buffer[i*2] = temp[i] & 0xFF; // младший байт
        buffer[i*2 + 1] = (temp[i] >> 8) & 0xFF; // старший байт
    }

    // Возвращаем статус ошибки: 0 ок, -1 ошибка
    return ata_get_err();
}

int ata_write_sector(unsigned int lba, unsigned char* src){
    // Проверяем и ждем готовность диска
    if (ata_wait_ready() < 0) return -1;

    // Выбираем диск Master + LBA (старший байт)
    outb(ATA_BASE + 6, 0xE0 | ((lba >> 24) & 0x0F));

    // Параметры
    outb(ATA_BASE + 2, 1);              // кол-во секторов: 1
    outb(ATA_BASE + 3, lba & 0xFF);     // LBA 0-7
    outb(ATA_BASE + 4, (lba >> 8) & 0xFF);  // LBA 8-15
    outb(ATA_BASE + 5, (lba >> 16) & 0xFF); // LBA 16-23

    // Команда записи
    outb(ATA_BASE + 7, 0x30);

    // Проверяем и ждем готовность диска
    if (ata_wait_ready() < 0) return -1;

    // переводим в массив short
    unsigned short temp[256];
    for (int i = 0; i < 256; i++) {
        temp[i] = src[i*2] | (src[i*2 + 1] << 8);  // младший байт | старший байт
    }

    // Записываем 512 байт (один сектор)
    ata_write(temp);

    // Проверяем и ждем готовность диска
    if (ata_wait_ready() < 0) return -1;

    // Возвращаем статус ошибки: 0 ок, -1 ошибка
    return ata_get_err();
}
*/

int ata_check_drive(unsigned int base, unsigned int drive){

    outb(base + REG_DEVICE, drive); // select drive
    in_out_wait();

    outb(base + REG_SECTOR_COUNT, 0);
    outb(base + REG_LBA_MID, 0);
    outb(base + REG_LBA_LOW, 0);
    outb(base + REG_LBA_HIGH, 0);

    outb(base + REG_COMMAND, ATA_CMD_IDENTIFY);

    uint8_t status = inb(base + REG_STATUS);
    if (status == 0) return -1; // нет устройства
    if (ata_wait_bsy(base) != 0) return -1; // timeout, нет устройства
    if (ata_wait_drq(base) != 0) return -1; // нет устройства
    if (ata_get_err(base) != 0) return -2; // error, нет устройства

    // устройство готово читаем identify
    uint8_t identify_buffer[512];
    ata_read(base, identify_buffer);

    uint16_t idf_dev_type = identify_buffer[0] | (identify_buffer[1] << 8);

    if (idf_dev_type == 0x0000 || idf_dev_type == 0xFFFF) return -1; // нет устройства

    return 0; // устройство существует

}


int ata_init(struct dev_info* device){

    unsigned int primary_io_base, primary_ctrl_base;
    unsigned int secondary_io_base, secondary_ctrl_base;

    struct pci_header_0_t* devh = (struct pci_header_0_t*)device->adv_info;

    bool primary_native   = (devh->common_header.prog_if & 0x01) != 0;
    bool secondary_native = (devh->common_header.prog_if & 0x04) != 0;

    if (!primary_native) { // Legacy
        primary_io_base   = 0x1F0;
        primary_ctrl_base = 0x3F6;
    }
    else { // PCI native
        primary_io_base   = devh->bar_resources[0].base;
        primary_ctrl_base = devh->bar_resources[1].base;
    }

    if (!secondary_native) { // Legacy
        secondary_io_base   = 0x170;
        secondary_ctrl_base = 0x376;
    }
    else { // PCI native
        secondary_io_base   = devh->bar_resources[2].base;
        secondary_ctrl_base = devh->bar_resources[3].base;
    }

    // Find and register devices

    if (ata_check_drive(primary_io_base, ATA_MASTER) == 0){
        kput("primary: master");
    }

    if (ata_check_drive(primary_io_base, ATA_SLAVE) == 0){
        kput("primary: slave");
    }

    if (ata_check_drive(secondary_io_base, ATA_MASTER) == 0){
        kput("secondary: master");
    }

    if (ata_check_drive(secondary_io_base, ATA_SLAVE) == 0){
        kput("secondary: slave");
    }

    while(1);
    // panic("ata", "test");
    return 1;
}
