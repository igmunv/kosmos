#include "io.h"
#include "syscall.h"
#include "device.h"


unsigned int get_default_output_device(){
    struct dev_info* devs = (struct dev_info*)_syscall(0x10A, 0, 0, 0, 0, 0);
    unsigned int dev_count = _syscall(0x109, 0, 0, 0, 0, 0);
    for (unsigned int dev = 0; dev < dev_count; dev++){
        if (devs[dev].classcode == VIRT_DISPLAY_CONTROLLER && devs[dev].subclass == VIRT_DISPLAY_VGATEXT){
            return dev;
        }
    }
}

void print_text(unsigned char* text, unsigned int size, unsigned char x, unsigned char y, unsigned char font_color, unsigned char bckg_color, unsigned int device_index){
    _syscall(
        SYS_PRINT_TEXT,
        text,
        size,
        (unsigned int)((x & 0xff) | ((y & 0xff) << 8)),
        (unsigned int)((font_color & 0xff) | ((bckg_color & 0xff) << 8)),
        device_index);
}

void print(unsigned char* text){
    // принимает строку с `\0` в конце
    unsigned int size;
    for (unsigned int i = 0; i < 2000; i++){
        if (text[i] == '\0'){
            size = i;
            break;
        }
    }
    if (size > 0){

        unsigned int device_index = get_default_output_device();
        unsigned char x = _syscall(SYS_GET_DISP_CURSOR_POS_X, device_index, 0, 0, 0, 0);
        unsigned char y = _syscall(SYS_GET_DISP_CURSOR_POS_Y, device_index, 0, 0, 0, 0);

        print_text(text, size, x, y, 7, 0, device_index);
    }
}
