DRIVER_NAME = keyboard
OBJ_DIR = ../../../output/drivers/$(DRIVER_NAME)

CC = i386-elf-gcc
CFLAGS = -w -ffreestanding -m32 -fno-pie -nostdlib

NSM = nasm
NSMFLAGS = -f elf32

all:
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c keyboard.c -o $(OBJ_DIR)/keyboard.o
	$(NSM) $(NSMFLAGS) keyboard.S -o $(OBJ_DIR)/keyboardS.o
