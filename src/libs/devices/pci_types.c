#include "pci_types.h"
#include "../string.h"

enum PCI_CLASSCODES{
    UNCLASSIFIED                        = 0x0,
    STORAGE_CONTROLLER                  = 0x1,
    NETWORK_CONTROLLER                  = 0x2,
    DISPLAY_CONTROLLER                  = 0x3,
    MULTIMEDIA_CONTROLLER               = 0x4,
    MEMORY_CONTROLLER                   = 0x5,
    BRIDGE                              = 0x6,
    SIMPLE_COMMUNICATION_CONTROLLER     = 0x7,
    BASE_SYSTEM_PERIPHERAL              = 0x8,
    INPUT_DEVICE_CONTROLLER             = 0x9,
    DOCKING_STATION                     = 0xA,
    PROCESSOR                           = 0xB,
    SERIAL_BUS_CONTROLLER               = 0xC,
    WIRELESS_CONTROLLER                 = 0xD,
    INTELLIGENT_CONTROLLER              = 0xE,
    SATELLITE_COMMUNICATION_CONTROLLER  = 0xF,
    ENCRYPTION_CONTROLLER               = 0x10,
    SIGNAL_PROCESSING_CONTROLLER        = 0x11,
    PROCESSING_ACCELERATOR              = 0x12,
    NON_ESSENTIAL_INSTRUMENTATION       = 0x13,
    PCI_CLASSCODES_COUNT
};

const unsigned char* PCI_CLASSCODE_NAMES[] = {
    [UNCLASSIFIED] = "UNCLASSIFIED",
    [STORAGE_CONTROLLER] = "STORAGE_CONTROLLER",
    [NETWORK_CONTROLLER] = "NETWORK_CONTROLLER",
    [DISPLAY_CONTROLLER] = "DISPLAY_CONTROLLER",
    [MULTIMEDIA_CONTROLLER] = "MULTIMEDIA_CONTROLLER",
    [MEMORY_CONTROLLER] = "MEMORY_CONTROLLER",
    [BRIDGE] = "BRIDGE",
    [SIMPLE_COMMUNICATION_CONTROLLER] = "SIMPLE_COMMUNICATION_CONTROLLER",
    [BASE_SYSTEM_PERIPHERAL] = "BASE_SYSTEM_PERIPHERAL",
    [INPUT_DEVICE_CONTROLLER] = "INPUT_DEVICE_CONTROLLER",
    [DOCKING_STATION] = "DOCKING_STATION",
    [PROCESSOR] = "PROCESSOR",
    [SERIAL_BUS_CONTROLLER] = "SERIAL_BUS_CONTROLLER",
    [WIRELESS_CONTROLLER] = "WIRELESS_CONTROLLER",
    [INTELLIGENT_CONTROLLER] = "INTELLIGENT_CONTROLLER",
    [SATELLITE_COMMUNICATION_CONTROLLER] = "SATELLITE_COMMUNICATION_CONTROLLER",
    [ENCRYPTION_CONTROLLER] = "ENCRYPTION_CONTROLLER",
    [SIGNAL_PROCESSING_CONTROLLER] = "SIGNAL_PROCESSING_CONTROLLER",
    [PROCESSING_ACCELERATOR] = "PROCESSING_ACCELERATOR",
    [NON_ESSENTIAL_INSTRUMENTATION] = "NON_ESSENTIAL_INSTRUMENTATION",
}

const unsigned char* PCI_TYPES[][] = {
    [UNCLASSIFIED] = {
        []
    }
    [STORAGE_CONTROLLER] = {
        []
    }
    [NETWORK_CONTROLLER] = {
        []
    }
    [DISPLAY_CONTROLLER] = {
        []
    }
    [MULTIMEDIA_CONTROLLER] = {
        []
    }
    [MEMORY_CONTROLLER] = {
        []
    }
    [BRIDGE] = {
        []
    }
    [SIMPLE_COMMUNICATION_CONTROLLER] = {
        []
    }
    [BASE_SYSTEM_PERIPHERAL] = {
        []
    }
    [INPUT_DEVICE_CONTROLLER] = {
        []
    }
    [DOCKING_STATION] = {
        []
    }
    [PROCESSOR] = {
        []
    }
    [SERIAL_BUS_CONTROLLER] = {
        []
    }
    [WIRELESS_CONTROLLER] = {
        []
    }
    [INTELLIGENT_CONTROLLER] = {
        []
    }
    [SATELLITE_COMMUNICATION_CONTROLLER] = {
        []
    }
    [ENCRYPTION_CONTROLLER] = {
        []
    }
    [SIGNAL_PROCESSING_CONTROLLER] = {
        []
    }
    [PROCESSING_ACCELERATOR] = {
        []
    }
    [NON_ESSENTIAL_INSTRUMENTATION] = {
        []
    }
    [UNASSIGNED_CLASS] = {
        []
    }

}

// возвращает размер строки
unsigned int get_pci_classcode_name(unsigned char classcode, unsigned char* dst){
    unsigned char* name;
    if (classcode > 0 && classcode < PCI_CLASSCODES_COUNT)
        name = PCI_CLASSCODE_NAMES[classcode];
    else // UNCLASSIFIED
        name = PCI_CLASSCODE_NAMES[0x0];
    int len = strlen(name);
    *dst = *name;
    return len;
}

// возвращает размер строки
unsigned int get_pci_subclass_name(){

}
