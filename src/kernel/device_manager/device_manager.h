#ifndef INCL_DEVICE_MANAGER
#define INCL_DEVICE_MANAGER

int devman_register_device(struct dev_info* device); // Регистрация устройства. Возвращает индекс устройства

void devman_unregister_device(unsigned int id); // Удаление устройства

struct dev_info* devman_get_devices(); // Получение массива с устройствами

unsigned int devman_get_device_count(); // Получение количества устройств

unsigned int devman_find_devices(); // Поиск устройств. Возвращает количество найденных устройств

#endif
