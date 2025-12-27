#include "time.h"
#include "device.h"

#include "../api/kernel_functions.h"

unsigned int time_get_ticks(){
    struct dev_info* pit = devman_get_first_device_by_specs(DEV_TYPE_LEG, LEG_PIT, 0);
    if (pit != 0)
        _get_ticks(pit->id);
}

void time_sleep(unsigned int ms){
    unsigned int wt = ms + time_get_ticks();
    while (time_get_ticks() <= wt);
}
