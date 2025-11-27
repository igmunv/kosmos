#define VIDEO_BUFFER_PTR (0xb8000)

#include "display.h"
#include "../../libs/device.h"

void* display_funcs[] = {
    display_print_text,
    display_new_line,
    display_cursor_update,
    display_clear,
    display_get_current_symbol,
    display_delete_current_symbol,
    display_get_display_cursor_pos_x,
    display_get_display_cursor_pos_y,
    display_set_display_cursor_pos_x,
    display_set_display_cursor_pos_y
};

// Нижний лимит по y
const char display_limit_x_bottom = 79;
const char display_limit_y_bottom = 24;

unsigned char DISPLAY_CURSOR_POS_X;
unsigned char DISPLAY_CURSOR_POS_Y;


// Обновить курсор
void display_cursor_update(struct dev_info* device){
    int display_cursor_location = DISPLAY_CURSOR_POS_Y * 80 + DISPLAY_CURSOR_POS_X;
    outb(0x3D4, 14);
    outb(0x3D5, display_cursor_location >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, display_cursor_location);
}


// Переход на новую (следующую) строку
void display_new_line(struct dev_info* device){
    if (DISPLAY_CURSOR_POS_Y >= display_limit_y_bottom){
        DISPLAY_CURSOR_POS_Y = 0;
        display_clear(device);
    }
    else DISPLAY_CURSOR_POS_Y++;

    DISPLAY_CURSOR_POS_X = 0;
    display_cursor_update(device);
}


// Вывести символ на экран
void display_print_symbol(unsigned char symbol, unsigned short x, unsigned short y, char font_color, char bkgr_color){
    unsigned char* video_mem = (unsigned char*)VIDEO_BUFFER_PTR;
    video_mem += (80 * y + x) * 2;
    video_mem[0] = symbol;
    video_mem[1] = font_color;
    bkgr_color = bkgr_color << 4;
    video_mem[1] = video_mem[1] | bkgr_color;
    video_mem += 2;
}


// Вывести текст на экран
void display_print_text(struct dev_info* device, unsigned char* text, unsigned int size, unsigned char x, unsigned char y, unsigned char font_color, unsigned char bkgr_color){

    DISPLAY_CURSOR_POS_X = x;
    DISPLAY_CURSOR_POS_Y = y;

    for (int i = 0; i < size; i++){
        display_print_symbol(text[i], DISPLAY_CURSOR_POS_X, DISPLAY_CURSOR_POS_Y, font_color, bkgr_color);

        if ((x + 1) > display_limit_x_bottom){
            DISPLAY_CURSOR_POS_Y++;
            DISPLAY_CURSOR_POS_X = 0;
        }
        else{
            DISPLAY_CURSOR_POS_X++;
        }

    }

}


// Очистить весь экран
void display_clear(struct dev_info* device){
    unsigned char* video_mem = (unsigned char*)VIDEO_BUFFER_PTR;
    for (unsigned short y = 0; y < 25; y++){
        for (unsigned short x = 0; x < 80; x++){
            video_mem[0] = 0;
            video_mem[1] = 7;
            video_mem += 2;
        }
    }

    DISPLAY_CURSOR_POS_X = 0;
    DISPLAY_CURSOR_POS_Y = 0;
    display_cursor_update(device);
}


// Получить текущий символ (можно со смещением)
unsigned char display_get_current_symbol(struct dev_info* device, short offset){
    unsigned char* video_mem = (unsigned char*)VIDEO_BUFFER_PTR;
	video_mem += (80 * (DISPLAY_CURSOR_POS_Y) + (DISPLAY_CURSOR_POS_X)) * 2;
    video_mem += 2 * offset;
    return video_mem[0];
}


// Удалить текущий символ (можно со смещением)
void display_delete_current_symbol(struct dev_info* device, short offset){
    unsigned char* video_mem = (unsigned char*)VIDEO_BUFFER_PTR;
	video_mem += (80 * (DISPLAY_CURSOR_POS_Y) + (DISPLAY_CURSOR_POS_X)) * 2;
    video_mem += 2 * offset;
    video_mem[0] = 0;
    video_mem[1] = 7;

    if (DISPLAY_CURSOR_POS_X == 0){
        if (DISPLAY_CURSOR_POS_Y > 0){
            DISPLAY_CURSOR_POS_Y--;
            DISPLAY_CURSOR_POS_X = 79;
            display_cursor_update(device);
        }
    }
    else{
        DISPLAY_CURSOR_POS_X--;
        display_cursor_update(device);
    }
}

unsigned char display_get_display_cursor_pos_x(struct dev_info* device){
    return DISPLAY_CURSOR_POS_X;
}

unsigned char display_get_display_cursor_pos_y(struct dev_info* device){
    return DISPLAY_CURSOR_POS_Y;
}

void display_set_display_cursor_pos_x(struct dev_info* device, unsigned char x){
    DISPLAY_CURSOR_POS_X = x;
}

void display_set_display_cursor_pos_y(struct dev_info* device, unsigned char y){
    DISPLAY_CURSOR_POS_Y = y;
}

int display_init(struct dev_info* device){
    return 1;
}
