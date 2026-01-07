#ifndef INCL_DRIVER_MBR
#define INCL_DRIVER_MBR

extern struct dev_info;
extern void* mbr_funcs[];
int mbr_init(struct dev_info* device);

#endif
