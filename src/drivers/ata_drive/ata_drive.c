#include "ata_drive.h"

#include "../../api/kernel_functions.h"

#include "../../libs/device.h"
#include "../../libs/io.h"
#include "../../libs/ata.h"
#include "../../libs/driver.h"

int ata_read_sector(struct dev_info* device, unsigned int lba, unsigned char* buffer);
int ata_write_sector(struct dev_info* device, unsigned int lba, unsigned char* src);

void* ata_drive_funcs[] =
{
    ata_read_sector,
    ata_write_sector
};


// Чтение определенного секотра
// lba - номер сектора
// buffer - буффер
int ata_read_sector(struct dev_info* device, unsigned int lba, unsigned char* buffer) {

    void (*ata_read)(unsigned int base, unsigned char* buffer) = (void (*)(unsigned int base, unsigned char* buffer))(device->parrent_dev->driver->funcs[0]); // ata_read
    void (*ata_write)(unsigned int base, unsigned char* buffer) = (void (*)(unsigned int base, unsigned char* buffer))(device->parrent_dev->driver->funcs[1]); // ata_write
    int (*ata_wait_bsy)(unsigned int base) = (int (*)(unsigned int base))(device->parrent_dev->driver->funcs[2]); // ata_wait_bsy
    int (*ata_wait_drq)(unsigned int base) = (int (*)(unsigned int base))(device->parrent_dev->driver->funcs[3]); // ata_wait_drq
    int (*ata_get_err)(unsigned int base) = (int (*)(unsigned int base))(device->parrent_dev->driver->funcs[4]); // ata_get_err

    // Выбираем диск Master + LBA (старший байт)
    struct ata_dev_info* adi = (struct ata_dev_info*)device->adv_info;

    uint8_t dev_bit = (adi->drive_type == SLAVE) ? 0x10 : 0x00;
    outb(adi->base + 6, 0xE0 | dev_bit | ((lba >> 24) & 0x0F));

    // Проверяем и ждем готовность диска
    if (ata_wait_bsy(adi->base) < 0) return -1;

    // Параметры
    outb(adi->base + 2, 1);              // кол-во секторов: 1
    outb(adi->base + 3, lba & 0xFF);     // LBA 0-7
    outb(adi->base + 4, (lba >> 8) & 0xFF);  // LBA 8-15
    outb(adi->base + 5, (lba >> 16) & 0xFF); // LBA 16-23

    // Команда чтения
    outb(adi->base + 7, 0x20);

    // Проверяем и ждем готовность диска
    if (ata_wait_bsy(adi->base) != 0) return -1;

    // Читаем 512 байт (один сектор)
    ata_read(adi->base, buffer);

    // Возвращаем статус ошибки: 0 ок, -1 ошибка
    return ata_get_err(adi->base);
}

int ata_write_sector(struct dev_info* device, unsigned int lba, unsigned char* src){

    void (*ata_read)(unsigned int base, unsigned char* buffer) = (void (*)(unsigned int base, unsigned char* buffer))(device->parrent_dev->driver->funcs[0]); // ata_read
    void (*ata_write)(unsigned int base, unsigned char* buffer) = (void (*)(unsigned int base, unsigned char* buffer))(device->parrent_dev->driver->funcs[1]); // ata_write
    int (*ata_wait_bsy)(unsigned int base) = (int (*)(unsigned int base))(device->parrent_dev->driver->funcs[2]); // ata_wait_bsy
    int (*ata_wait_drq)(unsigned int base) = (int (*)(unsigned int base))(device->parrent_dev->driver->funcs[3]); // ata_wait_drq
    int (*ata_get_err)(unsigned int base) = (int (*)(unsigned int base))(device->parrent_dev->driver->funcs[4]); // ata_get_err

    // Выбираем диск Master + LBA (старший байт)
    struct ata_dev_info* adi = (struct ata_dev_info*)device->adv_info;

    uint8_t dev_bit = (adi->drive_type == SLAVE) ? 0x10 : 0x00;
    outb(adi->base + 6, 0xE0 | dev_bit | ((lba >> 24) & 0x0F));

    // Проверяем и ждем готовность диска
    if (ata_wait_bsy(adi->base) < 0) return -1;

    // Параметры
    outb(adi->base + 2, 1);              // кол-во секторов: 1
    outb(adi->base + 3, lba & 0xFF);     // LBA 0-7
    outb(adi->base + 4, (lba >> 8) & 0xFF);  // LBA 8-15
    outb(adi->base + 5, (lba >> 16) & 0xFF); // LBA 16-23

    // Команда записи
    outb(adi->base + 7, 0x30);

    // Проверяем и ждем готовность диска
    if (ata_wait_bsy(adi->base) < 0) return -1;

    // Записываем 512 байт (один сектор)
    ata_write(adi->base, src);

    // Проверяем и ждем готовность диска
    if (ata_wait_bsy(adi->base) < 0) return -1;

    // Возвращаем статус ошибки: 0 ок, -1 ошибка
    return ata_get_err(adi->base);
}


int ata_drive_init(struct dev_info* device){
    _mbr_register_disk(device);
    return 1;
}

int ata_drive_probe(struct dev_info* device){
    return 1;
}

