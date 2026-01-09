#ifndef INCL_LIB_VFS
#define INCL_LIB_VFS

#include "device.h"

struct vfs_partition_info{
    unsigned int part_id;
    struct dev_info* device;
};

#endif
