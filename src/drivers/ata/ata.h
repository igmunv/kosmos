#ifndef INCL_DRIVER_ATA
#define INCL_DRIVER_ATA

extern struct dev_info;
extern void* ata_funcs[];

int ata_init(struct dev_info* device);

#endif
