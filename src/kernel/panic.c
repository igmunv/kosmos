#include "panic.h"
#include "../libs/asm.h"

#define VGA_MEM ((volatile unsigned short*)0xB8000)
#define VGA_COLS 80
#define VGA_ROWS 25

static unsigned char x = 0;
static unsigned char y = 0;

void panic_clear() {
    for (int i = 0; i < VGA_COLS * VGA_ROWS; i++)
        VGA_MEM[i] = (0x00 << 8) | ' ';
    x = 0;
    y = 0;
}

void panic_put(unsigned char* text) {
    const unsigned char font_color = 15;
    const unsigned char bkgr_color = 4;

    while (*text) {
        if (*text == '\n') {
            x = 0;
            y++;
        } else {
            volatile unsigned char* video_mem = (volatile unsigned char*)VGA_MEM;
            video_mem += (VGA_COLS * y + x) * 2;
            video_mem[0] = *text;
            video_mem[1] = (bkgr_color << 4) | font_color;

            x++;
            if (x >= VGA_COLS) {
                x = 0;
                y++;
            }
        }

        text++;

        if (y >= VGA_ROWS) {
            y = 0; // зацикливание если экран переполнен
        }
    }
}

void panic(unsigned char* where, unsigned char* text) {
    static int in_panic = 0;
    if (in_panic) goto halt; // защита от рекурсивного panic
    in_panic = 1;

    _intr_disable();

    panic_clear();
    panic_put((unsigned char*)"PANIC\n");
    panic_put(where);
    panic_put((unsigned char*)": ");
    panic_put(text);

    halt:
    while (1) {
        cpu_pause();
    }
}
