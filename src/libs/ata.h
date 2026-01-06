#ifndef INCL_LIB_ATA
#define INCL_LIB_ATA

#include "block.h"
#include "types.h"

enum ata_channel_type_enum{
    PRIMARY,
    SECONDARY
};

enum ata_drive_type_enum{
    MASTER,
    SLAVE
};

enum ata_type_enum{
    ATAPI,
    ATA
};

struct ata_dev_info{
    enum ata_type_enum type;
    uint8_t serial_number[21];
    uint8_t model_number[41];
    enum ata_channel_type_enum channel_type;
    enum ata_drive_type_enum drive_type;
    struct block_dev_info block_info;
};

#endif
