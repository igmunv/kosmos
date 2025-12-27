// GRUB header
__attribute__((section(".multiboot")))
const unsigned int multiboot_header[] = {
	0x1BADB002,
	0,
	-(0x1BADB002)
};

// Kernel
#include "gdt.h"
#include "IDT_PIC.h"

// api
#include "../api/api.h"
#include "../api/kernel_functions.h"

// libs
#include "../libs/asm.h"
#include "../libs/io.h"
#include "../libs/device.h"

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

// Main
void kmain(void){

	// GDT table init
	gdt_init();

	// Remap interrupts
	PIC_remap();

	// Init API
	api_init();

	// Find devices and drivers
	devman_find_devices();

	// log
	kput("Device Manager: ");
	unsigned char dev_count_str[10];
	int str_size = itos(devman_get_device_count(), dev_count_str);
	dev_count_str[str_size] = '\0';
	kput(dev_count_str);
	kput(" devices detected.\n");

	int found_drivers = 0;
	struct dev_info* devs = devman_get_devices();
	for (int i = 0; i < devman_get_device_count(); i++){
		if (devs[i].driver != 0)
			found_drivers++;
	}

	kput("Driver Manager: ");
	unsigned char drv_count_str[10];
	str_size = itos(found_drivers, drv_count_str);
	drv_count_str[str_size] = '\0';
	kput(drv_count_str);
	kput(" drivers successfully attached.\n");

	// Endless loop
	kernel_loop();
}
