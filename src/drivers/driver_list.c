
#include "driver_list.h"
#include "../libs/device.h"
#include "../libs/driver.h"

// - - Includes drivers - -
#include "pci/pci.h"
#include "pit/pit.h"
#include "keyboard/keyboard.h"
#include "display/display.h"
#include "ata/ata.h"

// - - Driver list - -
#define DRIVER_LIST \
    X(pci_driver, "pci.c", DEV_PCI, 0, 0, pci_init, pci_funcs) \
    X(pit_driver, "pit.c", DEV_TYPE_LEG, LEG_PIT, 0, pit_init, pit_funcs) \
    X(keyboard_driver, "keyboard.c", DEV_TYPE_LEG, LEG_PS2, LEG_PS2_keyboard, keyboard_init, keyboard_funcs) \
    X(display_driver, "display.c", DEV_TYPE_VIRT, VIRT_DISPLAY_CONTROLLER, VIRT_DISPLAY_VGATEXT, display_init, display_funcs) \
    X(ata_driver, "ata.c", DEV_TYPE_PCI, STORAGE_CONTROLLER, STORAGE_IDE_CONTROLLER, ata_init, ata_funcs)

// - - Template - -
#define X(name, filename, device_type, classcode, subclass, init, funcs) { #name, filename, device_type, classcode, subclass, init, funcs },

struct driver_info dl_drivers[] = {
    DRIVER_LIST
};


#undef X

#define X(name, filename, device_type, classcode, subclass, init, funcs) +1
const unsigned int dl_driver_count = 0 DRIVER_LIST;

#undef X
