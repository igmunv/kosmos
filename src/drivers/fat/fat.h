#ifndef INCL_DRIVER_FAT
#define INCL_DRIVER_FAT

extern struct dev_info;
extern void* fat_funcs[];
int fat_init(struct dev_info* device);
int fat_probe(struct dev_info* device);

#endif
