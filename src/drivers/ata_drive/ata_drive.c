#include "ata_drive.h"

#include "../../api/kernel_functions.h"

#include "../../libs/device.h"
#include "../../libs/io.h"
#include "../../libs/ata.h"

int ata_read_sector(struct dev_info* device, unsigned int lba, unsigned char* buffer);


void* ata_drive_funcs[] =
{
    ata_read_sector,
    ata_write_sector
};




// Чтение определенного секотра
// lba - номер сектора
// buffer - буффер
int ata_read_sector(struct dev_info* device, unsigned int lba, unsigned char* buffer) {

    // Проверяем и ждем готовность диска
    if (ata_wait_ready() < 0) return -1;

    // Выбираем диск Master + LBA (старший байт)
    struct ata_dev_info* adi = (struct ata_dev_info*)device->adv_info;

    uint8_t dev_bit = (adi->drive_type == SLAVE) ? 0x10 : 0x00;
    outb(adi->base + 6, 0xE0 | dev_bit | ((lba >> 24) & 0x0F));

    // Параметры
    outb(adi->base + 2, 1);              // кол-во секторов: 1
    outb(adi->base + 3, lba & 0xFF);     // LBA 0-7
    outb(adi->base + 4, (lba >> 8) & 0xFF);  // LBA 8-15
    outb(adi->base + 5, (lba >> 16) & 0xFF); // LBA 16-23

    // Команда чтения
    outb(adi->base + 7, 0x20);

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

int ata_write_sector(struct dev_info* device, unsigned int lba, unsigned char* src){
    // Проверяем и ждем готовность диска
    if (ata_wait_ready() < 0) return -1;

    // Выбираем диск Master + LBA (старший байт)
    struct ata_dev_info* adi = (struct ata_dev_info*)device->adv_info;

    uint8_t dev_bit = (adi->drive_type == SLAVE) ? 0x10 : 0x00;
    outb(adi->base + 6, 0xE0 | dev_bit | ((lba >> 24) & 0x0F));

    // Выбираем диск Master + LBA (старший байт)
    outb(adi->base + 6, 0xE0 | ((lba >> 24) & 0x0F));

    // Параметры
    outb(adi->base + 2, 1);              // кол-во секторов: 1
    outb(adi->base + 3, lba & 0xFF);     // LBA 0-7
    outb(adi->base + 4, (lba >> 8) & 0xFF);  // LBA 8-15
    outb(adi->base + 5, (lba >> 16) & 0xFF); // LBA 16-23

    // Команда записи
    outb(adi->base + 7, 0x30);

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


int ata_drive_init(struct dev_info* device){
    kput("drive\n");
    return 1;
}
