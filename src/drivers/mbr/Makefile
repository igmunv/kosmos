DRIVER_NAME = mbr
OBJ_DIR = ../../../output/drivers/$(DRIVER_NAME)

CC = i386-elf-gcc
CFLAGS = -w -ffreestanding -m32 -fno-pie -nostdlib

NSM = nasm
NSMFLAGS = -f elf32

all:
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c mbr.c -o $(OBJ_DIR)/mbr.o
