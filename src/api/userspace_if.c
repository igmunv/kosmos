// Интерфейс API функций для userspace

#include "api.h"
#include "userspace_if.h"
#include "general_functions.h"


void sys_print_text(struct registers_struct* args, struct registers_struct* result){

    _print_text(
                args->ebx,
                args->ecx,
                ((args->edx) & 0xFF),
                ((args->edx >> 8) & 0xFF),
                ((args->esi) & 0xFF),
                ((args->esi >> 8) & 0xFF),
                args->edi
                );

}

void sys_new_line(struct registers_struct* args, struct registers_struct* result){
    _new_line(args->ebx);
}

void sys_cursor_update(struct registers_struct* args, struct registers_struct* result){
    _cursor_update(args->ebx);
}

void sys_clear_display(struct registers_struct* args, struct registers_struct* result){
    _clear_display(args->ebx);
}

void sys_get_current_symbol(struct registers_struct* args, struct registers_struct* result){
    unsigned char symbol = _get_current_symbol(args->ebx, args->edx);
    result->eax = (unsigned int)symbol;
}

void sys_delete_current_symbol(struct registers_struct* args, struct registers_struct* result){
    unsigned char symbol = _delete_current_symbol(args->ebx, args->edx);
    result->eax = (unsigned int)symbol;
}

void sys_read_sector(struct registers_struct* args, struct registers_struct* result){
    unsigned char state = _read_sector(args->ebx, args->ecx, args->edx);
    result->eax = (unsigned int)state;
}

void sys_write_sector(struct registers_struct* args, struct registers_struct* result){
    unsigned char state = _write_sector(args->ebx, args->ecx, args->edx);
    result->eax = (unsigned int)state;
}

void sys_execute_program(struct registers_struct* args, struct registers_struct* result){
    _execute_program(args->ebx);
}

void sys_get_execute_program(struct registers_struct* args, struct registers_struct* result){
    unsigned int program = _get_execute_program();
    result->eax = (unsigned int)program;
}

void sys_get_keyboard_buffer(struct registers_struct* args, struct registers_struct* result){
    void* buffer = _get_keyboard_buffer(args->ebx);
    result->eax = (unsigned int)buffer;
}

void sys_get_keyboard_buffer_ptr(struct registers_struct* args, struct registers_struct* result){
    unsigned int buffer_ptr = _get_keyboard_buffer_ptr(args->ebx);
    result->eax = buffer_ptr;
}

void sys_get_keyboard_shift_pressed(struct registers_struct* args, struct registers_struct* result){
    unsigned char flag = _get_keyboard_shift_pressed(args->ebx);
    result->eax = (unsigned int)flag;
}

void sys_get_keyboard_ctrl_pressed(struct registers_struct* args, struct registers_struct* result){
    unsigned char flag = _get_keyboard_ctrl_pressed(args->ebx);
    result->eax = (unsigned int)flag;
}

void sys_get_keyboard_alt_pressed(struct registers_struct* args, struct registers_struct* result){
    unsigned char flag = _get_keyboard_alt_pressed(args->ebx);
    result->eax = (unsigned int)flag;
}

void sys_get_display_cursor_pos_x(struct registers_struct* args, struct registers_struct* result){
    unsigned int x = _get_display_cursor_pos_x(result->ebx);
    result->eax = x;
}

void sys_get_display_cursor_pos_y(struct registers_struct* args, struct registers_struct* result){
    unsigned int y = _get_display_cursor_pos_y(result->ebx);
    result->eax = y;
}

void sys_get_ticks(struct registers_struct* args, struct registers_struct* result){
    unsigned int ticks = _get_ticks();
    result->eax = ticks;
}

void sys_get_device_count(struct registers_struct* args, struct registers_struct* result){
    unsigned int count = devman_get_device_count();
    result->eax = count;
}

void sys_get_device_info(struct registers_struct* args, struct registers_struct* result){
    struct dev_info* devices = devman_get_devices();
    result->eax = (unsigned int)devices;
}

void sys_set_display_cursor_pos_x(struct registers_struct* args, struct registers_struct* result){
    _set_display_cursor_pos_x(result->ebx, result->ecx);
}

void sys_set_display_cursor_pos_y(struct registers_struct* args, struct registers_struct* result){
    _set_display_cursor_pos_x(result->ebx, result->ecx);
}
