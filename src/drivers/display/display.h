#ifndef INCL_DRIVER_DISPLAY
#define INCL_DRIVER_DISPLAY

extern struct dev_info;
extern void* display_funcs[];
int display_init(struct dev_info* device);

#endif
