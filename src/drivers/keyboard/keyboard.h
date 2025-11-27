#ifndef INCL_DRIVER_KEYBOARD
#define INCL_DRIVER_KEYBOARD

#include "../../libs/device.h"

extern void* keyboard_funcs[];
void* keyboard_get_buffer(struct dev_info* device);
unsigned int keyboard_get_buffer_ptr(struct dev_info* device);
unsigned char keyboard_get_shift_pressed(struct dev_info* device);
unsigned char keyboard_get_ctrl_pressed(struct dev_info* device);
unsigned char keyboard_get_alt_pressed(struct dev_info* device);
int keyboard_init(struct dev_info* device);

#endif
