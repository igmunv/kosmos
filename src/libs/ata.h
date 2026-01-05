#ifndef INCL_LIB_ATA
#define INCL_LIB_ATA

#include "block.h"

enum ata_channel_type_enum{
    PRIMARY,
    SECONDARY
};

enum ata_drive_type_enum{
    MASTER,
    SLAVE
};

struct ata_dev_info{
    unsigned char model[41];
    uint16_t type; // ata or atapi
    enum ata_channel_type_enum channel_type;
    enum ata_drive_type_enum drive_type;
    struct block_dev_info block_info;
};

#endif
