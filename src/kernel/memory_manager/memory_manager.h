#ifndef INCL_MEMORY_MANAGER
#define INCL_MEMORY_MANAGER

void* kmalloc(unsigned long long size);
void kfree(void* ptr);

#endif
