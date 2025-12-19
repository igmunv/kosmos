#define MAX_DEVICE_COUNT 256

#include "device_manager.h"
#include "../../api/kernel_functions.h"
#include "../../libs/memory.h"

struct dev_info DEVICES[MAX_DEVICE_COUNT];
unsigned int DEVICE_NEXT_INDEX;
unsigned int DEVICE_COUNT;
unsigned int FREE_SLOTS[MAX_DEVICE_COUNT];
unsigned int FREE_SLOTS_COUNT;

struct dev_info* PCI;
struct dev_info* PCI_DEVICES[MAX_DEVICE_COUNT];
unsigned int PCI_DEVICE_COUNT;
struct dev_info* LEG_DEVICES[MAX_DEVICE_COUNT];
unsigned int LEG_DEVICE_COUNT;
struct dev_info* VIRT_DEVICES[MAX_DEVICE_COUNT];
unsigned int VIRT_DEVICE_COUNT;

// Регистрация устройства. Возвращает индекс устройства
int devman_register_device(struct dev_info* device){

    unsigned int id;
    if (FREE_SLOTS_COUNT > 0){
        id = FREE_SLOTS[FREE_SLOTS_COUNT-1];
        device->id = id;
        device->is_free = 0;
        DEVICES[id] = *device;
        DEVICE_COUNT++;
        FREE_SLOTS_COUNT--;
    }
    else{
        id = DEVICE_NEXT_INDEX;
        device->id = id;
        device->is_free = 0;
        DEVICES[DEVICE_NEXT_INDEX] = *device;
        DEVICE_NEXT_INDEX++;
        DEVICE_COUNT++;
    }
    return id;
}

// Удаление устройства.
void devman_unregister_device(unsigned int id){
    DEVICES[id]->is_free = 1;
    FREE_SLOTS[FREE_SLOTS_COUNT] = id;
    FREE_SLOTS_COUNT++;
    DEVICE_COUNT--;
}

// Получение массива с устройствами
struct dev_info* devman_get_devices(){
    return &DEVICES;
}

// Получение устройств по типу. Возвращает количесто, в result указатель на массив устройств
unsigned int devman_get_devices_by_type(enum dev_types type, struct dev_info** result){

    switch(type){
        case DEV_PCI:
            *result = PCI;
            return 1;
        case DEV_TYPE_PCI:
            *result = PCI_DEVICES;
            return PCI_DEVICE_COUNT;
        case DEV_TYPE_LEG:
            *result = LEG_DEVICES;
            return LEG_DEVICE_COUNT;
        case DEV_TYPE_VIRT:
            *result = VIRT_DEVICES;
            return VIRT_DEVICE_COUNT;
        default:
            return -1; // unknown type
    }
}

// Получение количества устройств
unsigned int devman_get_device_count(){
    return DEVICE_COUNT;
}

void devman_find_virtual_devices(){
    // PCI
    // display vga text mode
    struct dev_info dev_display;
    dev_display.dev_type = DEV_TYPE_VIRT;

    dev_display.classcode = VIRT_DISPLAY_CONTROLLER;
    dev_display.subclass = VIRT_DISPLAY_VGATEXT;
    dev_display.vendor_id = 0;
    dev_display.device_id = 0;

    dev_display.driver = 0;

    devman_register_device(&dev_display);
}

void devman_find_legacy_devices(){
    // PIT
    // ps/2 keyboard
}

unsigned int devman_find_devices(){

    DEVICE_COUNT = 0;
    DEVICE_NEXT_INDEX = 0;
    FREE_SLOTS_COUNT = 0;

    devman_find_virtual_devices();
    devman_find_legacy_devices();
    _pci_find_devices(PCI->id);

}
