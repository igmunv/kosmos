DRIVER_NAME = pit
OBJ_DIR = ../../../output/drivers/$(DRIVER_NAME)

CC = i386-elf-gcc
CFLAGS = -w -ffreestanding -m32 -fno-pie -nostdlib

NSM = nasm
NSMFLAGS = -f elf32

all:
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c pit.c -o $(OBJ_DIR)/pit.o
	$(NSM) $(NSMFLAGS) pit_handler.asm -o $(OBJ_DIR)/pit_handler.o
