// GRUB header
__attribute__((section(".multiboot")))
const unsigned int multiboot_header[] = {
	0x1BADB002,
	0,
	-(0x1BADB002)
};

#include "kernel.h"
#include "timer.h"

// Kernel
#include "gdt.h"
#include "IDT_PIC.h"

// api
#include "../api/api.h"
#include "../api/kernel_functions.h"

#include "../libs/device.h"


#include "../libs/asm.h"


unsigned int EXECUTE_PROGRAM = 0;

// Loop
__attribute__((section(".kernel_loop"))) void kernel_loop(void) {
	while(1)
	{
		if (EXECUTE_PROGRAM == 0){

		}

		else{

			EXECUTE_PROGRAM = 0;
		}
	}
}


void test(){
	struct dev_info* devs = (struct dev_info*)_get_device_info();
    for (unsigned int dev = 0; dev < _get_device_count(); dev++){
        if (devs[dev].classcode == VIRT_DISPLAY_CONTROLLER && devs[dev].subclass == VIRT_DISPLAY_VGATEXT){

			unsigned char* text = "test";
			unsigned int size = 4;
			_print_text(text, size, 15, 2, 7, 0, dev);

        }
    }
}

// Main
void kmain(void){

	// GDT table init
	gdt_init();

	// Remap interrupts
	PIC_remap();

	// Init PIT, timers
	timer_init();

	// Init API
	api_init();

	devices_find();

	driver_manager();

	// test();

	// Endless loop
	kernel_loop();
}
