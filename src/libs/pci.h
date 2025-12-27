#ifndef INCL_LIB_PCI
#define INCL_LIB_PCI

struct pci_command_register {
    char io_space;
    char memory_space;
    char bus_master;
    char special_cycles;
    char mem_wr_and_inv_en;
    char vga_pal_sn;
    char par_err_resp;
    char serr_enable;
    char fast_bk_to_bk_en;
    char int_disable;
};

struct pci_status_register {
    char int_status;
    char capb_list;
    char _66mhz_cap;
    char fast_bk_to_bk_cap;
    char mast_dt_par_err;
    char devsel_timing;
    char sign_targ_abort;
    char received_targ_abort;
    char received_mast_abort;
    char sign_sys_err;
    char detect_par_err;
};

struct pci_common_header{
    unsigned short vendor_id;
    unsigned short device_id;
    unsigned short command;
    unsigned short status;
    unsigned char revision_id;
    unsigned char prog_if;
    unsigned char subclass;
    unsigned char classcode;
    unsigned char cache_line_size;
    unsigned char latency_timer;
    unsigned char mf;
    unsigned char header_type;
    unsigned char bist;
};

struct pci_header_0{
    struct pci_common_header common_header;
    unsigned int bar0;
    unsigned int bar1;
    unsigned int bar2;
    unsigned int bar3;
    unsigned int bar4;
    unsigned int bar5;
};

#endif
