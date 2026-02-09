#include "vfs.h"

#include "../../libs/vfs.h"

struct vfs_partition_info PARTITIONS[32] = {0};
unsigned int PARTITION_COUNT = 0;

// File Full Path: 0://file.txt
//                 PART_NUM://FILE


// func vfs_mount
int vfs_mount(struct dev_info* device){
    return 0;
}

// func vfs_read
int vfs_read(int part_id, unsigned char* file_name, unsigned char* dst){
    return 0;
}

// func vfs_write
int vfs_write(int part_id, unsigned char* file_name, unsigned char* src){
    return 0;
}

// func vfs_readdir
int vfs_readdir(unsigned char* dst){
    return 0;
}

// func vfs_getpart
int vfs_getpart(unsigned int* dst){
    return 0;
}
