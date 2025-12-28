#define MAX_DEVICE_COUNT 256

#include "device_manager.h"
#include "../../api/kernel_functions.h"
#include "../../libs/device.h"
#include "../../libs/array.h"

struct dev_info DEVICES[MAX_DEVICE_COUNT];
unsigned int DEVICE_NEXT_INDEX;
unsigned int DEVICE_COUNT;
unsigned int FREE_SLOTS[MAX_DEVICE_COUNT];
unsigned int FREE_SLOTS_COUNT;

unsigned int PCI = 0;
unsigned int PCI_DEVICES[MAX_DEVICE_COUNT];
unsigned int PCI_DEVICE_COUNT;
unsigned int LEG_DEVICES[MAX_DEVICE_COUNT];
unsigned int LEG_DEVICE_COUNT;
unsigned int VIRT_DEVICES[MAX_DEVICE_COUNT];
unsigned int VIRT_DEVICE_COUNT;

// Регистрация устройства. Возвращает индекс устройства
int devman_register_device(struct dev_info* device){

    unsigned int id; // id и index устройства в DEVICES

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

    switch(device->type){
        case DEV_PCI:
            PCI = id;
            break;
        case DEV_TYPE_PCI:
            PCI_DEVICES[PCI_DEVICE_COUNT] = id;
            PCI_DEVICE_COUNT++;
            break;
        case DEV_TYPE_LEG:
            LEG_DEVICES[LEG_DEVICE_COUNT] = id;
            LEG_DEVICE_COUNT++;
            break;
        case DEV_TYPE_VIRT:
            VIRT_DEVICES[VIRT_DEVICE_COUNT] = id;
            VIRT_DEVICE_COUNT++;
            break;
    }

    // Поиск драйвера
    drvman_attach_driver_to_device(&DEVICES[id]);

    return id;
}

// Удаление устройства.
void devman_unregister_device(unsigned int id){

    DEVICES[id].is_free = 1;
    FREE_SLOTS[FREE_SLOTS_COUNT] = id;
    FREE_SLOTS_COUNT++;
    DEVICE_COUNT--;

    switch(DEVICES[id].type){
        case DEV_TYPE_PCI:
            array_uint32_remove_element_by_value(PCI_DEVICES, &PCI_DEVICE_COUNT, id);
            break;
        case DEV_TYPE_LEG:
            array_uint32_remove_element_by_value(LEG_DEVICES, &LEG_DEVICE_COUNT, id);
            break;
        case DEV_TYPE_VIRT:
            array_uint32_remove_element_by_value(VIRT_DEVICES, &VIRT_DEVICE_COUNT, id);
            break;
    }
}

// Получение адреса массива со всеми устройствами
struct dev_info* devman_get_devices(){
    return DEVICES;
}

// Получение общего количества устройств
unsigned int devman_get_device_count(){
    return DEVICE_COUNT;
}

// Получение устройств по типу. Возвращает количество, в result указатель на массив индексов устройств
unsigned int devman_get_devices_by_type(enum dev_types type, unsigned int** result){

    switch(type){
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
            *result = 0;
            return 0; // unknown type
    }
}

// Получение устройства по идентификатору. Возвращает адрес на устройство
struct dev_info* devman_get_device_by_id(unsigned int id){
    return &(DEVICES[id]);
}

// Возвращает указатель на первое устройство, подходящее под параметры, или 0, если не найдено
struct dev_info* devman_get_first_device_by_specs(enum dev_types type, char classcode, char subclass){
    for (unsigned int i = 0; i < DEVICE_COUNT; i++){
        struct dev_info* device = &DEVICES[i];
        if (device->is_free) continue;
        if (device->type == type && device->classcode == classcode && device->subclass == subclass)
            return device;
    }
    return 0;
}

void devman_pci_bus_reg(){
    // PCI
    struct dev_info dev_pci = {0};
    dev_pci.type = DEV_PCI;
    int pci_id = devman_register_device(&dev_pci);
    if (DEVICES[pci_id].driver == 0){
        panic("devman_pci_bus_reg", "not found driver");
    }

}

void devman_find_virtual_devices(){
    // display vga text mode
    struct dev_info dev_display = {0};
    dev_display.type = DEV_TYPE_VIRT;
    dev_display.classcode = VIRT_DISPLAY_CONTROLLER;
    dev_display.subclass = VIRT_DISPLAY_VGATEXT;
    devman_register_device(&dev_display);
}

void devman_find_legacy_devices(){
    // PIT
    struct dev_info dev_pit = {0};
    dev_pit.type = DEV_TYPE_LEG;
    dev_pit.classcode = LEG_PIT;
    dev_pit.subclass = 0;
    devman_register_device(&dev_pit);

    // ps/2 keyboard
    struct dev_info dev_keyb = {0};
    dev_keyb.type = DEV_TYPE_LEG;
    dev_keyb.classcode = LEG_PS2;
    dev_keyb.subclass = LEG_PS2_keyboard;
    devman_register_device(&dev_keyb);
}

unsigned int devman_find_devices(){

    DEVICE_COUNT = 0;
    DEVICE_NEXT_INDEX = 0;
    FREE_SLOTS_COUNT = 0;

    devman_pci_bus_reg();
    devman_find_virtual_devices();
    devman_find_legacy_devices();
    if (DEVICES[PCI].type == DEV_PCI && DEVICES[PCI].driver != 0) _pci_find_devices(PCI);

    return DEVICE_COUNT;

}
