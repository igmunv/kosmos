#ifndef INCL_LIB_IO
#define INCL_LIB_IO

void kprinti(long number);
void kput(unsigned char* text);
void kprintc(unsigned char* text, unsigned char font, unsigned char bckg);
void kprintcp(unsigned char* text, unsigned char x, unsigned char y, unsigned char font, unsigned char bckg, char is_cursor_update);
void kclear();

#endif
