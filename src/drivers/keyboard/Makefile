include ../../../common.mk

DRIVER_NAME := keyboard
OBJ_DIR     := ../../../output/drivers/$(DRIVER_NAME)

all:
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c keyboard.c           -o $(OBJ_DIR)/keyboard.o
	$(AS) $(ASFLAGS)   keyboard_handler.asm -o $(OBJ_DIR)/keyboard_handler.o
