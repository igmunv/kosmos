#ifndef INCL_DRIVER_KEYBOARD
#define INCL_DRIVER_KEYBOARD

extern struct dev_info;
extern void* keyboard_funcs[];
int keyboard_init(struct dev_info* device);

#endif
