DRIVER_NAME = moonfs
OBJ_DIR = ../../../output/drivers/$(DRIVER_NAME)

CC = i386-elf-gcc
CFLAGS = -w -ffreestanding -m32 -fno-pie -nostdlib

NSM = nasm
NSMFLAGS = -f elf32

all:
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c moonfs.c -o $(OBJ_DIR)/moonfs.o