// Общие функции API

#include "general_functions.h"

#include "../libs/device.h"
#include "../libs/driver.h"

#include "../kernel/kernel.h"
#include "../kernel/timer.h"
#include "../drivers/devices.h"
#include "../drivers/drivers.h"

void _print_text(unsigned char* text, unsigned int size, unsigned char x, unsigned char y, unsigned char font_color, unsigned char bkgr_color, unsigned int device_index){

    struct dev_info* devices = _get_device_info();
    struct dev_info* device = &devices[device_index];

    void (*driver_print_text)(struct dev_info*, unsigned char*, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char) = (void (*)(struct dev_info*, unsigned char*, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char))(device->driver->funcs[PRINT_TEXT]);

    driver_print_text(device, text, size, x, y, font_color, bkgr_color);

}

void _new_line(unsigned int device_index){

    struct dev_info* devices = _get_device_info();
    struct dev_info* device = &devices[device_index];

    void (*driver_new_line)(struct dev_info*) = (void (*)(struct dev_info*))(device->driver->funcs[NEW_LINE]);

    driver_new_line(device);

}

void _cursor_update(unsigned int device_index){

    struct dev_info* devices = _get_device_info();
    struct dev_info* device = &devices[device_index];

    void (*driver_cursor_update)(struct dev_info*) = (void (*)(struct dev_info*))(device->driver->funcs[CURSOR_UPDATE]);

    driver_cursor_update(device);

}

void _clear_display(unsigned int device_index){

    struct dev_info* devices = _get_device_info();
    struct dev_info* device = &devices[device_index];

    void (*driver_clear_display)(struct dev_info*) = (void (*)(struct dev_info*))(device->driver->funcs[CLEAR_DISPLAY]);

    driver_clear_display(device);

}

unsigned char _get_current_symbol(unsigned int offset, unsigned int device_index){

    struct dev_info* devices = _get_device_info();
    struct dev_info* device = &devices[device_index];

    unsigned char (*driver_get_current_symbol)(struct dev_info*, unsigned int) = (unsigned char (*)(struct dev_info*, unsigned int))(device->driver->funcs[GET_CURRENT_SYMBOL]);

    unsigned char symbol = driver_get_current_symbol(device, offset);
    return symbol;

}

unsigned char _delete_current_symbol(unsigned int offset, unsigned int device_index){

    struct dev_info* devices = _get_device_info();
    struct dev_info* device = &devices[device_index];

    unsigned char (*driver_delete_current_symbol)(struct dev_info*, unsigned int) = (unsigned char (*)(struct dev_info*, unsigned int))(device->driver->funcs[DELETE_CURRENT_SYMBOL]);

    unsigned char symbol = driver_delete_current_symbol(device, offset);
    return symbol;

}

unsigned char _get_display_cursor_pos_x(unsigned int device_index){
    struct dev_info* devices = _get_device_info();
    struct dev_info* device = &devices[device_index];

    unsigned char (*driver_get_display_cursor_pos_x)(struct dev_info*) = (unsigned char (*)(struct dev_info*))(device->driver->funcs[GET_CURSOR_POS_X]);

    unsigned char symbol = driver_get_display_cursor_pos_x(device);
    return symbol;
}

unsigned char _get_display_cursor_pos_y(unsigned int device_index){
    struct dev_info* devices = _get_device_info();
    struct dev_info* device = &devices[device_index];

    unsigned char (*driver_get_display_cursor_pos_y)(struct dev_info*) = (unsigned char (*)(struct dev_info*))(device->driver->funcs[GET_CURSOR_POS_Y]);

    unsigned char symbol = driver_get_display_cursor_pos_y(device);
    return symbol;
}

void _set_display_cursor_pos_x(unsigned char x, unsigned int device_index){
    struct dev_info* devices = _get_device_info();
    struct dev_info* device = &devices[device_index];

    unsigned char (*driver_set_display_cursor_pos_x)(struct dev_info*, unsigned char) = (unsigned char (*)(struct dev_info*, unsigned char))(device->driver->funcs[SET_CURSOR_POS_X]);

    driver_set_display_cursor_pos_x(device, x);
}

void _set_display_cursor_pos_y(unsigned char y, unsigned int device_index){
    struct dev_info* devices = _get_device_info();
    struct dev_info* device = &devices[device_index];

    unsigned char (*driver_set_display_cursor_pos_y)(struct dev_info*, unsigned char) = (unsigned char (*)(struct dev_info*, unsigned char))(device->driver->funcs[SET_CURSOR_POS_Y]);

    driver_set_display_cursor_pos_y(device, y);
}

unsigned char _read_sector(unsigned int device_index, unsigned int lba, void* dst){

}

unsigned char _write_sector(unsigned int device_index, unsigned int lba, void* src){

}

void _execute_program(unsigned int program){
    EXECUTE_PROGRAM = program;
}

unsigned int _get_execute_program(){
    return EXECUTE_PROGRAM;
}

void* _get_keyboard_buffer(unsigned int device_index){
    struct dev_info* devices = _get_device_info();
    struct dev_info* device = &devices[device_index];
    void* (*get_keyboard_buffer)(struct dev_info*) = (void* (*)(struct dev_info*))(device->driver->funcs[GET_KEYBOARD_BUFFER]);
    return get_keyboard_buffer(device);
}

unsigned int _get_keyboard_buffer_ptr(unsigned int device_index){
    struct dev_info* devices = _get_device_info();
    struct dev_info* device = &devices[device_index];
    unsigned int (*get_keyboard_buffer_ptr)(struct dev_info*) = (unsigned int (*)(struct dev_info*))(device->driver->funcs[GET_KEYBOARD_BUFFER_PTR]);
    return get_keyboard_buffer_ptr(device);
}

unsigned char _get_keyboard_shift_pressed(unsigned int device_index){
    struct dev_info* devices = _get_device_info();
    struct dev_info* device = &devices[device_index];
    unsigned int (*get_keyboard_shift_pressed)(struct dev_info*) = (unsigned int (*)(struct dev_info*))(device->driver->funcs[GET_KEYBOARD_SHIFT_PRD]);
    return get_keyboard_shift_pressed(device);
}

unsigned char _get_keyboard_ctrl_pressed(unsigned int device_index){
    struct dev_info* devices = _get_device_info();
    struct dev_info* device = &devices[device_index];
    unsigned int (*get_keyboard_shift_pressed)(struct dev_info*) = (unsigned int (*)(struct dev_info*))(device->driver->funcs[GET_KEYBOARD_CTRL_PRD]);
    return get_keyboard_shift_pressed(device);
}

unsigned char _get_keyboard_alt_pressed(unsigned int device_index){
    struct dev_info* devices = _get_device_info();
    struct dev_info* device = &devices[device_index];
    unsigned int (*get_keyboard_alt_pressed)(struct dev_info*) = (unsigned int (*)(struct dev_info*))(device->driver->funcs[GET_KEYBOARD_ALT_PRD]);
    return get_keyboard_alt_pressed(device);
}



unsigned int _get_ticks(){
    return TICKS;
}

unsigned int _get_device_count(){
    return DEVICE_COUNT;
}



void* _get_device_info(){
    return &DEVICES_INFO;
}
