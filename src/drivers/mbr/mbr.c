#include "mbr.h"

#include "../../api/kernel_functions.h"

#include "../../libs/device.h"
#include "../../libs/io.h"
#include "../../libs/ata.h"
#include "../../libs/driver.h"

int mbr_register_disk(struct dev_info* disk_dev);

void* mbr_funcs[] = {
    mbr_register_disk
};

int mbr_register_disk(struct dev_info* disk_dev){
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
                // регистрация part_disk_dev
                kput("partition exists\n");
                kprinti(boot_flag);
                kput("\n");
                kprinti(part_type);
                kput("\n");
                kprinti(lba_start);
                kput("\n");
                kprinti(num_sectors);
                kput("\n");
            }
        }
    }
    while(1);
    return 0;
}

int mbr_init(struct dev_info* device){

    return 1;
}
