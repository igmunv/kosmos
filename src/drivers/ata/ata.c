#include "ata.h"
#include "../../libs/device.h"
#include "../../api/kernel_functions.h"

#include "../../libs/io.h"

#define ATA_BASE 1

void* ata_funcs[] = {

};


// стандартная задержка для ATA
void ata_delay(){
    inb(ATA_BASE + 7);
    inb(ATA_BASE + 7);
    inb(ATA_BASE + 7);
    inb(ATA_BASE + 7);
}

// Ожидание готовности принятия или отдачи данных и проверяет Занято ли устройство
int ata_wait_ready() {
    int timeout = 1000000;
    while (timeout-- > 0) {
        unsigned char status = inb(ATA_BASE + 7);

        if ((status & 0b10000000) == 0) {       // BSY=0
            // в QEMU не работают
            // if (status & 0x01) return -1; // ERR
            // if (status & 0x20) return -1; // Device Fault
            // if (status & 0b00001000) return 0;  // DRQ
            return 0;
        }
        in_out_wait();

    }
    return -1; // Timeout
}

int ata_get_err(){
    unsigned char status = inb(ATA_BASE + 7);
    if (status & 0x01) return -1; // ERR
    if (status & 0x20) return -1; // DF
    else return 0; // ok
}

// Читаем данные с диска
// Читаем 512 байт (один сектор)
void ata_read(unsigned short* buffer){
    for (int i = 0; i < 256; i++) {
        unsigned short word = inw(ATA_BASE);
        buffer[i] = word;
    }
}

// Записываем данные на диск
// Записываем 512 байт (один сектор)
void ata_write(unsigned short* src){
    for (int i = 0; i < 256; i++) {
        unsigned short word = src[i];
        outw(ATA_BASE, word);
    }
}


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


int ata_init(struct dev_info* device){

    unsigned int primary_io_base, primary_ctrl_base;
    unsigned int secondary_io_base, secondary_ctrl_base;

    unsigned char primary_prog_if   = device->prog_if & 0x0F; // низкие 4 бита
    unsigned char secondary_prog_if = (device->prog_if >> 4) & 0x0F; // высокие 4 бита

    // Primary
    if (primary_prog_if == 0x0) { // Legacy
        primary_io_base = 0x1F0;
        primary_ctrl_base = 0x3F6;
    } else { // Native PCI
        primary_io_base = device->bar_resources[0].base;
        primary_ctrl_base = device->bar_resources[1].base;
    }

    // Secondary
    if (secondary_prog_if == 0x0) { // Legacy
        secondary_io_base = 0x170;
        secondary_ctrl_base = 0x376;
    } else { // Native PCI
        secondary_io_base = device->bar_resources[2].base;
        secondary_ctrl_base = device->bar_resources[3].base;
    }
    kprinti(primary_io_base);
    while(1);
    panic("ata", "test");
    return 1;
}
