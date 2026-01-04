#include "memory_manager.h"
#include "../../libs/types.h"

#define ALIGN 8
#define ALIGN_UP(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

#define HEAP_SIZE (1024 * 1024)

unsigned char KHEAP[HEAP_SIZE];
unsigned int KHEAP_PTR = 0;

void* kmalloc(unsigned int size){
    size = ALIGN_UP(size, ALIGN);
    if (size + KHEAP_PTR > HEAP_SIZE) return NULL;
    void* ptr = &KHEAP[KHEAP_PTR];
    KHEAP_PTR += size;
    return ptr;
}

void kfree(void* ptr){
    return;
}
