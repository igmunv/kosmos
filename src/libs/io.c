#include "io.h"
#include "device.h"

#include "../api/kernel_functions.h"

void kput(unsigned char* text){
    struct dev_info* vga = devman_get_first_device_by_specs(DEV_TYPE_VIRT, DEV_UNKNOW, VIRT_DISPLAY_CONTROLLER, VIRT_DISPLAY_VGATEXT);
    if (vga != 0){
        unsigned int size = 65534;

        for (int i = 0; i < 65535; i++){
            if (text[i] == '\0'){
                size = i;
                break;
            }
        }

        unsigned char x = _get_display_cursor_pos_x(vga->id);
        unsigned char y = _get_display_cursor_pos_y(vga->id);
        _print_text(text, size, x, y, 7, 0, vga->id);
        _cursor_update(vga->id);
    }
}

void kprinti(long number){
    unsigned char str[10];
    int str_size = itos(number, str);
    str[str_size] = '\0';
    kput(str);
}

// print в котором можно управлять цветами
void kprintc(unsigned char* text, unsigned char font, unsigned char bckg){
    struct dev_info* vga = devman_get_first_device_by_specs(DEV_TYPE_VIRT, DEV_UNKNOW, VIRT_DISPLAY_CONTROLLER, VIRT_DISPLAY_VGATEXT);
    if (vga != 0){
        unsigned int size = 65534;

        for (int i = 0; i < 65535; i++){
            if (text[i] == '\0'){
                size = i;
                break;
            }
        }

        unsigned char x = _get_display_cursor_pos_x(vga->id);
        unsigned char y = _get_display_cursor_pos_y(vga->id);
        _print_text(text, size, x, y, font, bckg, vga->id);
        _cursor_update(vga->id);
    }
}

// print в котором можно управлять позицией и цветами
void kprintcp(unsigned char* text, unsigned char x, unsigned char y, unsigned char font, unsigned char bckg, char is_cursor_update){
    struct dev_info* vga = devman_get_first_device_by_specs(DEV_TYPE_VIRT, DEV_UNKNOW, VIRT_DISPLAY_CONTROLLER, VIRT_DISPLAY_VGATEXT);
    if (vga != 0){
        unsigned int size = 65534;

        for (int i = 0; i < 65535; i++){
            if (text[i] == '\0'){
                size = i;
                break;
            }
        }
        _print_text(text, size, x, y, font, bckg, vga->id);

        if (is_cursor_update == 1)
            _cursor_update(vga->id);
    }
}

void kclear(){
    struct dev_info* vga = devman_get_first_device_by_specs(DEV_TYPE_VIRT, DEV_UNKNOW, VIRT_DISPLAY_CONTROLLER, VIRT_DISPLAY_VGATEXT);
    if (vga != 0){
        _clear_display(vga->id);
    }
}
