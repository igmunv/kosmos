#include "vfs.h"

#include "../../libs/vfs.h"

struct vfs_partition_info PARTITIONS[32] = {0};
unsigned int PARTITION_COUNT = 0;

// File Full Path: 0://file.txt
//                 PART_NUM://FILE


// func vfs_mount
int vfs_mount(struct dev_info* device){

}

// func vfs_read
int vfs_read(int part_id, unsigned char file_name, unsigned char dst*){

}

// func vfs_write
int vfs_write(int part_id, unsigned char file_name, unsigned char src*){

}

// func vfs_readdir
int vfs_readdir(unsigned char dst*){

}

// func vfs_getpart
int vfs_getpart(unsigned int dst*){

}
