#define MAX_DRIVER_COUNT 256

#include "driver_manager.h"

#include "../../libs/device.h"
#include "../../libs/driver.h"

#include "../../drivers/driver_list.h"

// в DRIVERS лежат только те драйвера, которые используются на данный момент

struct driver_info DRIVERS[MAX_DRIVER_COUNT];
unsigned int DRIVER_COUNT;

// Регистрирует драйвер для конкретного устройства. возвращает индекс драйвера в массиве
int drvman_register_driver(struct driver_info* driver, struct dev_info* device){

    // здесь будет проблема с дублированием драйверов. нужно сделать проверку на уже существующий экземпляр драйвера в массиве DRIVERS
    DRIVERS[DRIVER_COUNT] = *driver;
    device->driver = &DRIVERS[DRIVER_COUNT];
    DRIVER_COUNT++;

    int (*probe)(struct dev_info*) = (int (*)(struct dev_info*))(driver->probe);
    int (*init)(struct dev_info*) = (int (*)(struct dev_info*))(driver->init);
    if (probe != 0) init(device);

    return DRIVER_COUNT-1;
}

// Находит подходящий драйвер для устройства. Записывает информацию о драйвере в result. Возвращает 0 если не нашел 1 если нашел
int drvman_get_device_driver(struct dev_info* device, struct driver_info* result){

    // Пока что просто читает из статического списка dl_drivers,
    // который собирается при компиляции.
    // Потом будет читать файлы.

    for(int driver_index = 0; driver_index < dl_driver_count; driver_index++){

        struct driver_info* driver = &dl_drivers[driver_index];
        if ((driver->con_type == device->con_type) && (driver->classcode == device->classcode) && (driver->subclass == device->subclass)){

            int (*probe)(struct dev_info*) = (int (*)(struct dev_info*))(driver->probe);
            int (*init)(struct dev_info*) = (int (*)(struct dev_info*))(driver->init);
            int probe_result = 0;
            if (probe == 0) probe_result = init(device);
            else probe_result = probe(device);
            if (probe_result == 1){
                *result = *driver;
                return 1;
            }
        }
    }

    // Здесь будет поиск драйверов в файловой системе

    return 0; // Not Found

}

// Поиск драйвера для устройства, а затем привязка. Возвращает 0 если драйвер не найден, 1 если найден
int drvman_attach_driver_to_device(struct dev_info* device){
    struct driver_info driver;
    if (drvman_get_device_driver(device, &driver)){
        drvman_register_driver(&driver, device);
        return 1;
    }
    else return 0;
}
