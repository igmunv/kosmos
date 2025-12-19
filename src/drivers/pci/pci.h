#ifndef INCL_DRIVER_PCI
#define INCL_DRIVER_PCI

extern struct dev_info;
extern void* pci_funcs[];
int pci_init(struct dev_info* device);

#endif
