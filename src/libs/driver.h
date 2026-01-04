#ifndef INCL_LIB_DRIVER
#define INCL_LIB_DRIVER

#define DRIVER_FUNC_COUNT 16

#include "device.h"

struct driver_info {

    unsigned char name[16];
    unsigned char filename[16];

    enum dev_con_types con_type;
    unsigned char classcode;
    unsigned char subclass;

    void* init;
    void** funcs;

};


enum dfunctype_virt_display_vgatext{
    PRINT_TEXT,
    NEW_LINE,
    CURSOR_UPDATE,
    CLEAR_DISPLAY,
    GET_CURRENT_SYMBOL,
    DELETE_CURRENT_SYMBOL,
    GET_CURSOR_POS_X,
    GET_CURSOR_POS_Y,
    SET_CURSOR_POS_X,
    SET_CURSOR_POS_Y,
};

enum dfunctype_virt_ps2_keyb{
    GET_KEYBOARD_BUFFER,
    GET_KEYBOARD_BUFFER_PTR,
    GET_KEYBOARD_SHIFT_PRD,
    GET_KEYBOARD_CTRL_PRD,
    GET_KEYBOARD_ALT_PRD
};

enum dfunctype_virt_pci{
    PCI_FIND_DEVICES,
    PCI_CONF_READ_WORD,
    PCI_CONF_READ_DWORD,
    PCI_CONF_WRITE_WORD,
    PCI_CONF_GET_COMMON_HEADER,
    PCI_CONF_GET_HEADER_0,
    PCI_CONF_GET_COMMAND,
    PCI_CONF_GET_STATUS,
    PCI_CONF_SET_COMMAND
};

#endif
