#ifndef KERNEL
#define KERNEL

extern unsigned int EXECUTE_PROGRAM;

extern void _syscall(unsigned int syscall_number, unsigned int arg1, unsigned int arg2, unsigned int arg3, unsigned int arg4, unsigned int arg5);

void kmain(void);

#endif
