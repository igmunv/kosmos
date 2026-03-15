include ../../../common.mk

DRIVER_NAME := pit
OBJ_DIR     := ../../../output/drivers/$(DRIVER_NAME)

all:
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c pit.c           -o $(OBJ_DIR)/pit.o
	$(AS) $(ASFLAGS)   pit_handler.asm -o $(OBJ_DIR)/pit_handler.o
