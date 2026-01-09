#include "fat.h"

#include "../../api/kernel_functions.h"

#include "../../libs/device.h"
#include "../../libs/driver.h"
#include "../../libs/partition.h"
#include "../../libs/io.h"
#include "../../libs/memory.h"
#include "../../libs/string.h"
#include "../../libs/fat12.h"

int vfs_read(unsigned char file_name, unsigned char dst*);
int vfs_write(unsigned char file_name, unsigned char src*);
int vfs_readdir(unsigned char dst*);
int vfs_getpart(unsigned int dst*);

void* fat_funcs[] = {

};

int fat_init(struct dev_info* device){
    // определение файловой системы FAT12. если не определена то возврат 0, определена 1
    struct part_disk_info* pdi = (struct part_disk_info*)device->adv_info;
    unsigned char first_sector[512];
    _read_sector(pdi->parrent_disk->id, pdi->lba_start, first_sector);

    unsigned char fs_type[9] = {0};
    memcpy(fs_type, &first_sector[54], 8);
    fs_type[8] = 0;

    if (!streql("FAT12   ", fs_type, 8)) return 0; // не подходит

    struct fat12_info* f12i = kmalloc(sizeof(struct fat12_info));
    memcpy(f12i->fs_label, fs_type, 8);

    // BPB
    f12i->bytes_per_sector    = first_sector[11] | (first_sector[12] << 8);
    f12i->sectors_per_cluster = first_sector[13];
    f12i->reserved_sectors    = first_sector[14] | (first_sector[15] << 8);
    f12i->num_fats            = first_sector[16];
    f12i->root_entry_count    = first_sector[17] | (first_sector[18] << 8);
    f12i->total_sectors16     = first_sector[19] | (first_sector[20] << 8);
    f12i->sectors_per_fat     = first_sector[22] | (first_sector[23] << 8);
    f12i->total_sectors32     = first_sector[32] | (first_sector[33] << 8) |(first_sector[34] << 16) | (first_sector[35] << 24);

    unsigned int total_sectors = f12i->total_sectors16 ? f12i->total_sectors16 : f12i->total_sectors32;

    // вычисляем смещения LBA
    f12i->fat_start_lba  = f12i->reserved_sectors; // FAT сразу после зарезервированных секторов
    f12i->root_start_lba = f12i->fat_start_lba + f12i->num_fats * f12i->sectors_per_fat;

    // размер root в секторах
    unsigned int root_dir_sectors = ((f12i->root_entry_count * 32) + (f12i->bytes_per_sector - 1)) / f12i->bytes_per_sector;

    f12i->data_start_lba       = f12i->root_start_lba + root_dir_sectors;
    f12i->total_data_clusters  = (total_sectors - f12i->data_start_lba) / f12i->sectors_per_cluster;

    f12i->fat_cache      = NULL;
    f12i->root_dir_cache = NULL;


    return 1;
}

int fat_probe(struct dev_info* device){
    return 1;
}
