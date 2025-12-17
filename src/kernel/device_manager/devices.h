#ifndef INCL_DRIVER_DEVICES
#define INCL_DRIVER_DEVICES

#include "../libs/device.h"

extern struct dev_info DEVICES_INFO[256];
extern unsigned int DEVICE_COUNT;

void device_registration();

int device_manager();

#endif
