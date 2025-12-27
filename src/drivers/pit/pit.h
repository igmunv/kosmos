#ifndef INCL_DRIVER_PIT
#define INCL_DRIVER_PIT

extern struct dev_info;
extern void* pit_funcs[];
int pit_init(struct dev_info* device);

#endif
