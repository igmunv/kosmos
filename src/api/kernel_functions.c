// Функции API только для kernel space

#include "kernel_functions.h"
#include "../kernel/IDT_PIC.h"
#include "../libs/driver.h"

void _int_reg_handler(int number, unsigned short segm_sel, unsigned short flags, intr_handler handler){
    interrupt_disable();
    IDT_reg_handler(number, segm_sel, flags, handler);
    IDT_load();
    interrupt_enable();
}

void _pic_update_mask(unsigned char mst_sl, unsigned char bit, unsigned char value){
    PIC_update_mask(mst_sl, bit, value);
}

void _intr_enable(){
    interrupt_enable();
}

void _intr_disable(){
    interrupt_disable();
}

void _pci_find_devices(unsigned int device_index){
    struct dev_info* devices = devman_get_devices();
    struct dev_info* device = &devices[device_index];

    void (*driver_pci_find_devices)(void) = (void (*)(void))(device->driver->funcs[PCI_FIND_DEVICES]);

    driver_pci_find_devices();
}

unsigned short _pci_config_read_word(unsigned int bus, unsigned int dev, unsigned int func, unsigned char offset, unsigned int device_index){
    struct dev_info* devices = devman_get_devices();
    struct dev_info* device = &devices[device_index];

    unsigned short (*driver_pci_config_read_word)(unsigned int, unsigned int, unsigned int, unsigned char) = (unsigned short (*)(unsigned int, unsigned int, unsigned int, unsigned char))(device->driver->funcs[PCI_CONF_READ_WORD]);

    return driver_pci_config_read_word(bus, dev, func, offset);
}

unsigned int _pci_config_read_dword(unsigned int bus, unsigned int dev, unsigned int func, unsigned char offset, unsigned int device_index){
    struct dev_info* devices = devman_get_devices();
    struct dev_info* device = &devices[device_index];

    unsigned int (*driver_pci_config_read_dword)(unsigned int, unsigned int, unsigned int, unsigned char) = (unsigned int (*)(unsigned int, unsigned int, unsigned int, unsigned char))(device->driver->funcs[PCI_CONF_READ_DWORD]);

    return driver_pci_config_read_dword(bus, dev, func, offset);
}

void _pci_config_write_word(unsigned int bus, unsigned int dev, unsigned int func, unsigned char offset, unsigned short word, unsigned int device_index){
    struct dev_info* devices = devman_get_devices();
    struct dev_info* device = &devices[device_index];

    void (*driver_pci_config_write_word)(unsigned int, unsigned int, unsigned int, unsigned char, unsigned short) = (void (*)(unsigned int, unsigned int, unsigned int, unsigned char, unsigned short))(device->driver->funcs[PCI_CONF_WRITE_WORD]);

    driver_pci_config_write_word(bus, dev, func, offset, word);
}

struct pci_common_header _pci_config_get_common_header(unsigned int bus, unsigned int dev, unsigned int func, unsigned int device_index){
    struct dev_info* devices = devman_get_devices();
    struct dev_info* device = &devices[device_index];

    struct pci_common_header (*driver_pci_config_get_common_header)(unsigned int, unsigned int, unsigned int) = (struct pci_common_header (*)(unsigned int, unsigned int, unsigned int))(device->driver->funcs[PCI_CONF_GET_COMMON_HEADER]);

    return driver_pci_config_get_common_header(bus, dev, func);
}

struct pci_header_0_t _pci_config_get_header_0(unsigned int bus, unsigned int dev, unsigned int func, unsigned int device_index){
    struct dev_info* devices = devman_get_devices();
    struct dev_info* device = &devices[device_index];

    struct pci_header_0_t (*driver_pci_config_get_header_0)(unsigned int, unsigned int, unsigned int) = (struct pci_header_0_t (*)(unsigned int, unsigned int, unsigned int))(device->driver->funcs[PCI_CONF_GET_HEADER_0]);

    return driver_pci_config_get_header_0(bus, dev, func);
}

struct pci_command_register _pci_config_get_command(unsigned int bus, unsigned int dev, unsigned int func, unsigned int device_index){
    struct dev_info* devices = devman_get_devices();
    struct dev_info* device = &devices[device_index];

    struct pci_command_register (*driver_pci_config_get_command)(unsigned int, unsigned int, unsigned int) = (struct pci_command_register (*)(unsigned int, unsigned int, unsigned int))(device->driver->funcs[PCI_CONF_GET_COMMAND]);

    return driver_pci_config_get_command(bus, dev, func);
}

struct pci_status_register _pci_config_get_status(unsigned int bus, unsigned int dev, unsigned int func, unsigned int device_index){
    struct dev_info* devices = devman_get_devices();
    struct dev_info* device = &devices[device_index];

    struct pci_status_register (*driver_pci_config_get_status)(unsigned int, unsigned int, unsigned int) = (struct pci_status_register (*)(unsigned int, unsigned int, unsigned int))(device->driver->funcs[PCI_CONF_GET_STATUS]);

    return driver_pci_config_get_status(bus, dev, func);
}

void _pci_config_set_command(unsigned int bus, unsigned int dev, unsigned int func, struct pci_command_register cmd, unsigned int device_index){
    struct dev_info* devices = devman_get_devices();
    struct dev_info* device = &devices[device_index];

    void (*driver_pci_config_set_command)(unsigned int, unsigned int, unsigned int, struct pci_command_register) = (void (*)(unsigned int, unsigned int, unsigned int, struct pci_command_register))(device->driver->funcs[PCI_CONF_SET_COMMAND]);

    driver_pci_config_set_command(bus, dev, func, cmd);
}

int _mbr_register_disk(struct dev_info* disk_dev){
    struct dev_info* mbr = devman_get_first_device_by_specs(DEV_TYPE_VIRT, DEV_UNKNOW, VIRT_MBR, 0);
    if (mbr != 0){
        int (*driver_mbr_register_disk)(struct dev_info*, struct dev_info*) = (int (*)(struct dev_info*, struct dev_info*))(mbr->driver->funcs[0]);
        return driver_mbr_register_disk(mbr, disk_dev);
    }
}
