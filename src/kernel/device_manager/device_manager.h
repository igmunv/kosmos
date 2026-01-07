#ifndef INCL_DEVICE_MANAGER
#define INCL_DEVICE_MANAGER

#include "../../libs/types.h"
#include "../../libs/device.h"

extern struct dev_info;
extern enum dev_con_types;

int devman_register_device(struct dev_info* device); // Регистрация устройства. Возвращает индекс устройства

void devman_unregister_device(unsigned int id); // Удаление устройства

struct dev_info* devman_get_devices(); // Получение массива с устройствами

unsigned int devman_get_device_count(); // Получение количества устройств

unsigned int devman_get_devices_by_con_type(enum dev_con_types con_type, unsigned int** result); // Получение устройств по типу подключения. Возвращает количество, в result указатель на массив индексов устройств

unsigned int devman_get_devices_by_type(enum dev_types type, unsigned int** result); // Получение устройств по типу. Возвращает количество, в result указатель на массив индексов устройств

struct dev_info* devman_get_device_by_id(unsigned int id); // Получение устройства по идентификатору. Возвращает адрес на устройство

struct dev_info* devman_get_first_device_by_specs(enum dev_con_types con_type, enum dev_types type, uint8_t classcode, uint8_t subclass); // Возвращает указатель на первое устройство, подходящее под параметры, или 0, если не найдено

unsigned int devman_find_devices(); // Поиск устройств. Возвращает количество найденных устройств

#endif
