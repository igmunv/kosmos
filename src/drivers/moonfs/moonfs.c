#include "moonfs.h"
#include "../../libs/device.h"
#include "../../libs/driver.h"
#include "../../libs/partition.h"
#include "../../libs/string.h"
#include "../../libs/moonfs.h"
#include "../../libs/memory.h"


struct moonfs_service SERVICE = {0};
struct moonfs_file_record FILE_RECORDS_TABLE[MOONFS_FILE_RECORD_COUNT] = {0};
unsigned short FILES_COUNT = 0;
struct dev_info* PARRENT_DEV = 0;




void* moonfs_funcs[] = {
    
};


// Получить список файлов
void moonfs_ls(){
	
}

// Получить дескриптор файла
// Если файла нет: -1
// Иначе дескриптор возращен
unsigned int moonfs_desc(unsigned char* name){
	for (unsigned int i = 0; i <= FILES_COUNT; i++){
		struct moonfs_file_record* file_record = &FILE_RECORDS_TABLE[i]; // xz pointer
		if (is_str_equally(file_record->name, name)) return i;
	}	
	return -1;
}

// Прочитать файл
void* moonfs_read(unsigned int desc){
	struct moonfs_file_record* file_record = &FILE_RECORDS_TABLE[desc];
	void* buffer = kmalloc(file_record->size);
	unsigned int current_buf_pointer = 0;
	unsigned long long current_sector = file_record->first_sector;
	for (int i = 0; i <= file_record->sector_count; i++){
		struct dev_info* ata_drive = PARRENT_DEV->parrent_dev;
		struct driver_info* ata_drive_driver = ata_drive->driver;
		int (*ata_read)(struct dev_info*, unsigned int, unsigned char*) = (int (*)(struct dev_info*, unsigned int, unsigned char*))(ata_drive_driver->funcs[0]);

		unsigned char* ata_buffer[512];
		ata_read(ata_drive, current_sector, ata_buffer);
		memcpy(&current_sector, &ata_buffer[503], 8);
		memcpy(buffer+current_buf_pointer, &ata_buffer, 504);
		current_buf_pointer += 504;
	}
	return buffer;
}

// Записать в файл
void moonfs_write(unsigned int desc, unsigned char* data, unsigned int size){
	
}

// Читает служебный раздел
void moonfs_read_service(){
	
}

// Читает таблицу файлов
void moonfs_read_frec_table(){
	
}


void moonfs_probe(struct dev_info* device){

	struct part_disk_info* pdi = (struct part_disk_info*)device->adv_info;
	
	if (pdi->part_type != 0x0D) return 0;

    return 1;
}

void moonfs_init(struct dev_info* device){
	PARRENT_DEV = device;
	return 1;
}
	