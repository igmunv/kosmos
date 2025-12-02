#include "devices.h"
#include "../libs/device.h"
#include "../libs/string.h"
#include "pci/pci.h"

#include "drivers.h"


struct dev_info DEVICES_INFO[256];
unsigned int DEVICE_COUNT;


void device_log(struct dev_info* device) {
    unsigned int dev = 0;

    unsigned char x = _get_display_cursor_pos_x(dev);
    unsigned char y = _get_display_cursor_pos_y(dev);

    unsigned char type_fcolor = 2;
    unsigned char tire_fcolor = 7;
    unsigned char class_fcolor = 7;
    unsigned char subclass_fcolor = 7;

    // Тип устройства
    if (device->is_pci_dev == 1) {

        _print_text("pci", 3, x, y, type_fcolor, 0, dev);

        x = _get_display_cursor_pos_x(dev);
        y = _get_display_cursor_pos_y(dev);

        _print_text(": ", 2, x, y, tire_fcolor, 0, dev);

        unsigned char bus_string[3];
        int bus_string_size = itos(device->bus, bus_string);
        if (bus_string_size == 1){
            x = _get_display_cursor_pos_x(dev);
            y = _get_display_cursor_pos_y(dev);
            _print_text("0", 1, x, y, tire_fcolor, 0, dev);
        }
        x = _get_display_cursor_pos_x(dev);
        y = _get_display_cursor_pos_y(dev);
        _print_text(bus_string, bus_string_size, x, y, tire_fcolor, 0, dev);
        x = _get_display_cursor_pos_x(dev); y = _get_display_cursor_pos_y(dev); _print_text(":", 1, x, y, tire_fcolor, 0, dev);

        unsigned char dev_string[3];
        int dev_string_size = itos(device->dev, dev_string);
        if (dev_string_size == 1){
            x = _get_display_cursor_pos_x(dev);
            y = _get_display_cursor_pos_y(dev);
            _print_text("0", 1, x, y, tire_fcolor, 0, dev);
        }
        x = _get_display_cursor_pos_x(dev);
        y = _get_display_cursor_pos_y(dev);
        _print_text(dev_string, dev_string_size, x, y, tire_fcolor, 0, dev);

        x = _get_display_cursor_pos_x(dev); y = _get_display_cursor_pos_y(dev); _print_text(".", 1, x, y, tire_fcolor, 0, dev);

        unsigned char func_string[3];
        int func_string_size = itos(device->func, func_string);
        if (bus_string_size == 1){
            x = _get_display_cursor_pos_x(dev);
            y = _get_display_cursor_pos_y(dev);
            _print_text("0", 1, x, y, tire_fcolor, 0, dev);
        }
        x = _get_display_cursor_pos_x(dev);
        y = _get_display_cursor_pos_y(dev);
        _print_text(func_string, func_string_size, x, y, tire_fcolor, 0, dev);

        x = _get_display_cursor_pos_x(dev); y = _get_display_cursor_pos_y(dev); _print_text(" ", 1, x, y, tire_fcolor, 0, dev);


        unsigned int pci_class_num = 0;
        unsigned int print_flag = 0;

        for (;;){

            if (pci_class_names[pci_class_num].name == 0) break;
            if (pci_class_names[pci_class_num].code == device->classcode){
                x = _get_display_cursor_pos_x(dev);
                y = _get_display_cursor_pos_y(dev);
                unsigned int size = strlen(pci_class_names[pci_class_num].name);
                _print_text(pci_class_names[pci_class_num].name, size, x, y, class_fcolor, 0, dev);
                print_flag = 1;
            }

            x = _get_display_cursor_pos_x(dev);
            y = _get_display_cursor_pos_y(dev);

            pci_class_num++;
        }

        // Класс не найден
        if (print_flag == 0){
            x = _get_display_cursor_pos_x(dev);
            y = _get_display_cursor_pos_y(dev);
            _print_text("UNKNOWN", 7, x, y, class_fcolor, 0, dev);
        }

        x = _get_display_cursor_pos_x(dev);
        y = _get_display_cursor_pos_y(dev);

        _print_text(" ", 1, x, y, tire_fcolor, 0, dev);

        unsigned int pci_subclass_m_num = 0;
        for (;;){

            struct subclass_map* subclass_m = pci_subclass_names[pci_subclass_m_num];

            // Конец масива
            if (pci_subclass_names[pci_subclass_m_num] == 0) break;

            unsigned int pci_subclass_num = 0;
            for (;;){

                // Конец массива
                if (subclass_m[pci_subclass_num].name == 0) break;

                if (device->classcode == subclass_m[pci_subclass_num].classcode && device->subclass == subclass_m[pci_subclass_num].subclass){
                    x = _get_display_cursor_pos_x(dev);
                    y = _get_display_cursor_pos_y(dev);
                    unsigned int size = strlen(subclass_m[pci_subclass_num].name);
                    _print_text(subclass_m[pci_subclass_num].name, size, x, y, subclass_fcolor, 0, dev);
                }

                pci_subclass_num++;
            }

            pci_subclass_m_num++;
        }

    } else if (device->is_leg_dev == 1) {
        _print_text("legacy", 6, x, y, type_fcolor, 0, dev);
    } else if (device->is_virt_dev == 1) {
        _print_text("virt", 4, x, y, type_fcolor, 0, dev);
    }

    x = _get_display_cursor_pos_x(dev); y = _get_display_cursor_pos_y(dev); _print_text(" (", 2, x, y, tire_fcolor, 0, dev);

    unsigned char vendorid_string[5];
    int vendorid_string_size = itos(device->vendor_id, vendorid_string);
    x = _get_display_cursor_pos_x(dev);
    y = _get_display_cursor_pos_y(dev);
    _print_text(vendorid_string, vendorid_string_size, x, y, tire_fcolor, 0, dev);

    x = _get_display_cursor_pos_x(dev); y = _get_display_cursor_pos_y(dev); _print_text(", ", 2, x, y, tire_fcolor, 0, dev);

    unsigned char deviceid_string[5];
    int deviceid_string_size = itos(device->device_id, deviceid_string);
    x = _get_display_cursor_pos_x(dev);
    y = _get_display_cursor_pos_y(dev);
    _print_text(deviceid_string, deviceid_string_size, x, y, tire_fcolor, 0, dev);

    x = _get_display_cursor_pos_x(dev); y = _get_display_cursor_pos_y(dev); _print_text(")", 1, x, y, tire_fcolor, 0, dev);

    _new_line(dev);
}



void device_registration(struct dev_info* device){
    DEVICES_INFO[DEVICE_COUNT] = *device;
    DEVICE_COUNT++;
    if(
        DEVICE_COUNT == 1 &&
        DEVICES_INFO[0].classcode == VIRT_DISPLAY_CONTROLLER &&
        DEVICES_INFO[0].subclass == VIRT_DISPLAY_VGATEXT
    ) driver_manager();  // устройство дисплей => сразу же ищем ему драйвер (для логов)
    if (
        DEVICE_COUNT > 0 &&
        DEVICES_INFO[0].classcode == VIRT_DISPLAY_CONTROLLER &&
        DEVICES_INFO[0].subclass == VIRT_DISPLAY_VGATEXT
    ) device_log(device); // если есть устройство дисплей, то выводим логи
}


//виртуальные устройства
void devices_virt(){

    // display vga text mode
    struct dev_info dev_display;
    dev_display.is_pci_dev = 0;
    dev_display.is_virt_dev = 1;
    dev_display.is_leg_dev = 0;

    dev_display.classcode = VIRT_DISPLAY_CONTROLLER;
    dev_display.subclass = VIRT_DISPLAY_VGATEXT;
    dev_display.vendor_id = 0;
    dev_display.device_id = 0;

    dev_display.driver = 0;

    device_registration(&dev_display);

}


void devices_legacy_find(){

    // keyboard
    struct dev_info dev_keyb;
    dev_keyb.is_pci_dev = 0;
    dev_keyb.is_virt_dev = 0;
    dev_keyb.is_leg_dev = 1;

    dev_keyb.classcode = VIRT_KEYBOARD_CONTROLLER;
    dev_keyb.subclass = VIRT_KEYBOARD_LDEV;
    dev_keyb.vendor_id = 0;
    dev_keyb.device_id = 0;

    dev_keyb.driver = 0;

    device_registration(&dev_keyb);

}


int device_manager(){

    unsigned int old_device_count = DEVICE_COUNT;

    devices_virt();
    devices_legacy_find();
    pci_find_devices();

    // Возвращает то, сколько нашёл устройств
    return (DEVICE_COUNT-old_device_count);
}
