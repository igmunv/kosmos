#ifndef INCL_DRIVER_DISPLAY
#define INCL_DRIVER_DISPLAY

extern struct dev_info;
extern void* display_funcs[];

void display_cursor_update(struct dev_info* device);

void display_new_line(struct dev_info* device);

void display_print_text(struct dev_info* device, unsigned char* text, unsigned int size, unsigned char x, unsigned char y, unsigned char font_color, unsigned char bkgr_color);

void display_clear(struct dev_info*);

unsigned char display_get_current_symbol(struct dev_info* device, short offset);

void display_delete_current_symbol(struct dev_info* device, short offset);

unsigned char display_get_display_cursor_pos_x(struct dev_info* device);

unsigned char display_get_display_cursor_pos_y(struct dev_info* device);

void display_set_display_cursor_pos_x(struct dev_info* device, unsigned char x);

void display_set_display_cursor_pos_y(struct dev_info* device, unsigned char y);

int display_init(struct dev_info* device);


#endif
