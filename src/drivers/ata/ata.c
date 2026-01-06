#include "ata.h"

#include "../../api/kernel_functions.h"

#include "../../libs/device.h"
#include "../../libs/io.h"
#include "../../libs/types.h"
#include "../../libs/ata.h"
#include "../../libs/asm.h"
#include "../../libs/string.h"

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

void ata_read(unsigned int base, unsigned char* buffer);
void ata_write(unsigned int base, const unsigned char* buffer);
int ata_wait_bsy(unsigned int base);
int ata_wait_drq(unsigned int base);
int ata_get_err(unsigned int base);

void* ata_funcs[] = {
    ata_read,
    ata_write,
    ata_wait_bsy,
    ata_wait_drq,
    ata_get_err
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
void ata_write(unsigned int base, const unsigned char* buffer){
    for (int i = 0; i < 256; i++) {
        uint16_t word = buffer[i*2] | (buffer[i*2 + 1] << 8);
        outw(base, word);
    }
}

int ata_get_identify(unsigned int base, unsigned int drive, struct ata_dev_info* drive_info){

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
    else if (idf_dev_type & 0x8000) return -1; // ATAPI
    else drive_info->type = ATA;

    uint16_t w49 = identify_buffer[49*2] | (identify_buffer[49*2+1] << 8);
    if (!(w49 & (1 << 9))) return -1; // not LBA

    int k = 0;
    for (int i = 20; i < 40; i += 2) {
        drive_info->serial_number[k++] = identify_buffer[i + 1];
        drive_info->serial_number[k++] = identify_buffer[i];
    }
    drive_info->serial_number[20] = 0;

    k = 0;
    for (int i = 54; i < 94; i += 2) {
        drive_info->model_number[k++] = identify_buffer[i + 1];
        drive_info->model_number[k++] = identify_buffer[i];
    }
    drive_info->model_number[40] = 0;

    drive_info->block_info.sector_count =
    ((uint32_t)identify_buffer[61*2+1] << 24) |  // старший байт word 61
    ((uint32_t)identify_buffer[61*2]   << 16) |  // младший байт word 61
    ((uint32_t)identify_buffer[60*2+1] << 8)  |  // старший байт word 60
    ((uint32_t)identify_buffer[60*2]);          // младший байт word 60

    uint16_t w106 = identify_buffer[106*2] | (identify_buffer[106*2+1] << 8);
    drive_info->block_info.sector_size = 512; // по умолчанию

    if (w106 & (1 << 14)) { // valid
        if (w106 & (1 << 12))
            drive_info->block_info.sector_size = 4096;
        else
            drive_info->block_info.sector_size = 512;
    }

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
    uint32_t base;
    uint32_t drive;
    for (int i = 0; i < 4; i++){

        struct ata_dev_info* ata_drive_info = NULL;
        ata_drive_info = kmalloc(sizeof(struct ata_dev_info));

        switch(i){
            case 0:
                base = primary_io_base;
                drive = ATA_MASTER;
                ata_drive_info->channel_type = PRIMARY;
                ata_drive_info->drive_type = MASTER;
                break;
            case 1:
                base = primary_io_base;
                drive = ATA_SLAVE;
                ata_drive_info->channel_type = PRIMARY;
                ata_drive_info->drive_type = SLAVE;
                break;
            case 2:
                base = secondary_io_base;
                drive = ATA_MASTER;
                ata_drive_info->channel_type = SECONDARY;
                ata_drive_info->drive_type = MASTER;
                break;
            case 3:
                base = secondary_io_base;
                drive = ATA_SLAVE;
                ata_drive_info->channel_type = SECONDARY;
                ata_drive_info->drive_type = SLAVE;
                break;

        }

        ata_drive_info->base = base;

        if (ata_get_identify(base, drive, ata_drive_info) != 0) continue; // device not found

        struct dev_info ata_drive = {0};
        ata_drive.con_type = DEV_TYPE_VIRT;
        ata_drive.classcode = VIRT_STORAGE_CONTROLLER;
        ata_drive.subclass = VIRT_STORAGE_ATA_DRIVE;
        ata_drive.adv_info = (void*)ata_drive_info;
        ata_drive.parrent_dev = device;

        devman_register_device(&ata_drive);

    }

    while(1);
    // panic("ata", "test");
    return 1;
}
