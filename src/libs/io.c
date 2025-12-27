#include "io.h"
#include "device.h"

#include "../api/kernel_functions.h"

void kput(unsigned char* text){
    struct dev_info* vga = devman_get_first_device_by_specs(DEV_TYPE_VIRT, VIRT_DISPLAY_CONTROLLER, VIRT_DISPLAY_VGATEXT);
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
