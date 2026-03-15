include ../../common.mk

DRIVER_DIRS   := $(filter-out .,$(wildcard */))
DRIVER_OUTPUT := ../../output/drivers/

all: drivers

# Файлы прямо в этой папке (если появятся)
%.o: %.c
	mkdir -p $(DRIVER_OUTPUT)
	$(CC) $(CFLAGS) -c $< -o $(DRIVER_OUTPUT)$@

%.o: %.asm
	mkdir -p $(DRIVER_OUTPUT)
	$(AS) $(ASFLAGS) $< -o $(DRIVER_OUTPUT)$@

# Сборка драйверов в подпапках
drivers:
	@for dir in $(DRIVER_DIRS); do \
		if [ -f $$dir/Makefile ]; then \
			echo "==> Building driver in $$dir"; \
			$(MAKE) -C $$dir; \
		else \
			echo "==> Skipping $$dir (no Makefile)"; \
		fi; \
	done
