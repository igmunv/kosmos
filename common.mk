# Путь до корня проекта, независимо от того, откуда включают
ROOT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# ==============================================================================
# Toolchain
# ==============================================================================
CROSS   ?= i386-elf
PREFIX  := $(CROSS)-

CC      := $(PREFIX)gcc
LD      := $(PREFIX)ld
AR      := $(PREFIX)ar
NM      := $(PREFIX)nm
OBJCOPY := $(PREFIX)objcopy
OBJDUMP := $(PREFIX)objdump
STRIP   := $(PREFIX)strip
AS      := nasm

# ==============================================================================
# Flags
# ==============================================================================
CFLAGS  := -w -ffreestanding -m32 -fno-pie -nostdlib
LDFLAGS := -m elf_i386 -T $(ROOT_DIR)src/linker.ld --oformat elf32-i386
ASFLAGS := -f elf32
