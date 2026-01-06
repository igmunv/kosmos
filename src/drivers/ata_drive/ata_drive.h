#ifndef INCL_DRIVER_ATA_DRIVE
#define INCL_DRIVER_ATA_DRIVE

extern struct dev_info;
extern void* ata_drive_funcs[];
int ata_drive_init(struct dev_info* device);

#endif
