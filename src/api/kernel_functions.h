// Функции API только для kernel space

#ifndef INCL_API_KF
#define INCL_API_KF

typedef void (*intr_handler)();

void _int_reg_handler(int number, unsigned short segm_sel, unsigned short flags, intr_handler handler);

// value: 0 - включает, 1 - выключает
void _pic_update_mask(unsigned char mst_sl, unsigned char bit, unsigned char value);

void _intr_enable();

void _intr_disable();

void _pci_find_devices(unsigned int device_index);

unsigned short _pci_config_read_word(unsigned int bus, unsigned int dev, unsigned int func, unsigned char offset, unsigned int device_index);

unsigned int _pci_config_read_dword(unsigned int bus, unsigned int dev, unsigned int func, unsigned char offset, unsigned int device_index);

void _pci_config_write_word(unsigned int bus, unsigned int dev, unsigned int func, unsigned char offset, unsigned short word, unsigned int device_index);

struct pci_common_header _pci_config_get_common_header(unsigned int bus, unsigned int dev, unsigned int func, unsigned int device_index);

struct pci_header_0 _pci_config_get_header_0(unsigned int bus, unsigned int dev, unsigned int func, unsigned int device_index);

struct pci_command_register _pci_config_get_command(unsigned int bus, unsigned int dev, unsigned int func, unsigned int device_index);

struct pci_status_register _pci_config_get_status(unsigned int bus, unsigned int dev, unsigned int func, unsigned int device_index);

void _pci_config_set_command(unsigned int bus, unsigned int dev, unsigned int func, struct pci_command_register cmd, unsigned int device_index);

#include "general_functions.h"

#endif
