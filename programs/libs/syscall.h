#ifndef INCL_LIB_SYSCALL
#define INCL_LIB_SYSCALL

#define SYS_PRINT_TEXT 0x0
#define SYS_NEW_LINE 0x1
#define SYS_CURSOR_UPDATE 0x2
#define SYS_CLEAR_DISPLAY 0x3
#define SYS_GET_CURRENT_SYMB 0x4
#define SYS_DEL_CURRENT_SYMB 0x5
#define SYS_GET_DISP_CURSOR_POS_X 0x106
#define SYS_GET_DISP_CURSOR_POS_Y 0x107
#define SYS_SET_DISP_CURSOR_POS_X 0x200
#define SYS_SET_DISP_CURSOR_POS_Y 0x201


extern unsigned int _syscall(unsigned int syscall_number, unsigned int arg1, unsigned int arg2, unsigned int arg3, unsigned int arg4, unsigned int arg5);


#endif

