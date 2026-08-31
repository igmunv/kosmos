#ifndef INCL_LIB_MOONFS
#define INCL_LIB_MOONFS

#define MOONFS_GLOBAL_FILE_COUNT_FIELD_SIZE 2
#define MOONFS_FILE_NAME_SIZE 16
#define MOONFS_FILE_RECORD_COUNT 65536

struct moonfs_file_record {
	unsigned char is_busy;
	unsigned char name[16];
	unsigned long long size;
	unsigned long long first_sector;
	unsigned long long sector_count;
};

struct moonfs_service {
	unsigned short file_count;
	unsigned long long available_memory;
};

#endif