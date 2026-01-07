#include "mbr.h"

#include "../../api/kernel_functions.h"

#include "../../libs/device.h"
#include "../../libs/io.h"
#include "../../libs/ata.h"
#include "../../libs/driver.h"
#include "../../libs/partition.h"

int mbr_register_disk(struct dev_info* mbr_dev, struct dev_info* disk_dev);

void* mbr_funcs[] = {
    mbr_register_disk
};

int mbr_register_disk(struct dev_info* mbr_dev, struct dev_info* disk_dev){
    unsigned char sector_buffer[512] = {0};
    _read_sector(disk_dev->id, 0, sector_buffer);
    if (sector_buffer[510] == 0x55 && sector_buffer[511] == 0xAA){
        // MBR is exists
        for (int part_num = 0; part_num < 4; part_num++){
            unsigned int addr = 446 + (16 * part_num);
            unsigned char mbr_partition_record[16];
            for (unsigned int i = 0; i < 16; i++) mbr_partition_record[i] = sector_buffer[(addr+i)];

            unsigned char boot_flag = mbr_partition_record[0];
            unsigned char part_type = mbr_partition_record[4];
            unsigned int lba_start = mbr_partition_record[11] << 24 | mbr_partition_record[10] << 16 | mbr_partition_record[9] << 8 | mbr_partition_record[8];
            unsigned int num_sectors = mbr_partition_record[15] << 24 | mbr_partition_record[14] << 16 | mbr_partition_record[13] << 8 | mbr_partition_record[12];

            if (part_type != 0 || num_sectors > 0 || lba_start > 0){
                // раздел существует
                // регистрация part_disk
                struct part_disk_info* part_disk = kmalloc(sizeof(struct part_disk_info));
                part_disk->parrent_disk = disk_dev;
                part_disk->lba_start = lba_start;
                part_disk->num_sectors = num_sectors;
                part_disk->part_type = part_type;
                part_disk->boot_flag = boot_flag;

                struct dev_info part_disk_dev = {
                    .parrent_dev = mbr_dev,
                    .con_type = DEV_TYPE_VIRT,
                    .type = DEV_BLOCK,
                    .classcode = VIRT_PARTITION,
                    .adv_info = (void*)part_disk
                };

                devman_register_device(&part_disk_dev);
            }
        }
    }
    return 0;
}

int mbr_init(struct dev_info* device){

    return 1;
}
