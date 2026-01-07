#ifndef INCL_LIB_PARTITION
#define INCL_LIB_PARTITION

#include "device.h"

struct part_disk_info{
    struct dev_info* parrent_disk;
    unsigned int lba_start;
    unsigned int num_sectors;
    unsigned char part_type;
    unsigned char boot_flag;
    void* fs_data;
};

#endif
