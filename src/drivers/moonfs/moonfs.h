#ifndef INCL_DRIVER_MOONFS
#define INCL_DRIVER_MOONFS

extern struct dev_info;
extern void* moonfs_funcs[];
void moonfs_probe(struct dev_info* device);
void moonfs_init(struct dev_info* device);

#endif