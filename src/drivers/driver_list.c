
#include "driver_list.h"
#include "../libs/device.h"
#include "../libs/driver.h"

// - - Includes drivers - -
#include "pci/pci.h"
#include "keyboard/keyboard.h"
#include "display/display.h"

// - - Driver list - -
#define DRIVER_LIST \
    X(pci_driver, "pci.c", DEV_TYPE_VIRT, VIRT_PCI, 0, pci_init, pci_funcs) \
    X(keyboard_driver, "keyboard.c", DEV_TYPE_LEG, VIRT_KEYBOARD_CONTROLLER, VIRT_KEYBOARD_LDEV, keyboard_init, keyboard_funcs) \
    X(display_driver, "display.c", DEV_TYPE_VIRT, VIRT_DISPLAY_CONTROLLER, VIRT_DISPLAY_VGATEXT, display_init, display_funcs)

// - - Template - -
#define X(name, filename, device_type, classcode, subclass, init, funcs) { #name, filename, device_type, classcode, subclass, init, funcs },

struct driver_info drivers[] = {
    DRIVER_LIST
};


#undef X

#define X(name, filename, device_type, classcode, subclass, init, funcs) +1
const unsigned int driver_count = 0 DRIVER_LIST;

#undef X
