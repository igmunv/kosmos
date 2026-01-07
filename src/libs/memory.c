#include "memory.h"

void memcpy(void* dst, void* src, int size){
    for (int i = 0; i < size; i++){
        char* src_byte = src+i;
        char* dst_byte = dst+i;
        *dst_byte = *src_byte;
    }
}

void memset(void* dst, unsigned char value, int count){
    for (int i = 0; i < count; i++){
        ((unsigned char*)dst)[i] = value;
    }
}
