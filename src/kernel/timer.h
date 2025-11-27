#ifndef INCL_DRIVER_PIT
#define INCL_DRIVER_PIT

extern unsigned int TICKS;

void timer_init();
void tick_handler();

#endif
