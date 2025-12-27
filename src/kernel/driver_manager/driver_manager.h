#ifndef INCL_DRIVER_MANAGER
#define INCL_DRIVER_MANAGER

extern struct dev_info;

// Поиск драйвера для устройства, а затем привязка. Возвращает 0 если драйвер не найден, 1 если найден
int drvman_attach_driver_to_device(struct dev_info* device);

#endif
