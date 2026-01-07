#ifndef INCL_LIB_FAT12
#define INCL_LIB_FAT12

#include "device.h"

struct fat12_info {

    // BPB — поля из boot sector
    uint16_t bytes_per_sector;    // 0x0B
    uint8_t  sectors_per_cluster; // 0x0D
    uint16_t reserved_sectors;    // 0x0E
    uint8_t  num_fats;            // 0x10
    uint16_t root_entry_count;    // 0x11
    uint16_t sectors_per_fat;     // 0x16
    uint16_t total_sectors16;     // 0x13
    uint32_t total_sectors32;     // 0x20

    // вычисленные значения
    uint32_t fat_start_lba;       // LBA начала первой FAT
    uint32_t root_start_lba;      // LBA начала root directory
    uint32_t data_start_lba;      // LBA начала области данных
    uint32_t total_data_clusters; // количество кластеров в data area

    // кеши / динамические данные
    void* fat_cache;              // кеш FAT (опционально)
    void* root_dir_cache;         // кеш root directory (опционально)

    unsigned char     fs_label[12];        // volume label, 11 символов + null

};

#endif
