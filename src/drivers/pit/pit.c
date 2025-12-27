#define PIT_CMD  0x43
#define PIT_CH0  0x40
#define PIT_INPUT_FREQ 1193182UL

#include "pit.h"
#include "../../api/kernel_functions.h"
#include "../../libs/device.h"

unsigned int pit_get_ticks(struct dev_info* device);

void* pit_funcs[] = {
    pit_get_ticks;
};

extern void asm_tick_handler();

unsigned int TICKS = 0;

// Инициализация таймера PIT
void PIT_init(unsigned int freq_hz) {

    // Выставляем заданную в freq_hz частоту (количество тиков в секунду)

    unsigned int divisor = (unsigned int)(PIT_INPUT_FREQ / freq_hz); // 1193182 / freq
    unsigned char lo = divisor & 0xFF;
    unsigned char hi = (divisor >> 8) & 0xFF;

    // Mode 3 (square wave), access lobyte/hibyte, channel 0
    outb(PIT_CMD, 0x36);

    // write divisor low then high
    outb(PIT_CH0, lo);
    outb(PIT_CH0, hi);
}

// Обработчик прерывания PIT
void tick_handler(){
	TICKS++;
	outb(0x20, 0x20);
}

unsigned int pit_get_ticks(struct dev_info* device){
    return TICKS;
}

int pit_init(struct dev_info* device){
    PIT_init(1000);
    _intr_disable();
    _pic_update_mask(0, 0, 0);
    _int_reg_handler(32, 0x08, 0x80 | 0x0E, asm_tick_handler);
    _intr_enable();
    return 1;
}
