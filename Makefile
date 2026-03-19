include common.mk

# ==============================================================================
# Sources
# ==============================================================================
LIBS_C   := $(shell find src/libs   -type f -name '*.c')
LIBS_S   := $(shell find src/libs   -type f -name '*.asm')
KERNEL_C := $(shell find src/kernel -type f -name '*.c')
KERNEL_S := $(shell find src/kernel -type f -name '*.asm')
API_C    := $(shell find src/api    -type f -name '*.c')
API_S    := $(shell find src/api    -type f -name '*.asm')

OBJS     := $(shell mkdir -p output; find output -name '*.o')

# ==============================================================================
# Build rule helper
# ==============================================================================
define build-rule
	for src in $(1); do \
		obj=$$(echo $$src | sed 's|$(2)|$(3)|' | sed 's|$(4)$$|$(5)|'); \
		echo "Building $$src -> $$obj"; \
		mkdir -p $$(dirname $$obj); \
		if echo $$src | grep -q "\.c$$"; then \
			$(CC) $(CFLAGS) -c $$src -o $$obj; \
		else \
			$(AS) $(ASFLAGS) $$src -o $$obj; \
		fi; \
	done
endef

# ==============================================================================
# Targets
# ==============================================================================
.PHONY: all build drivers libs kernel k_api link clean make_iso make_disk run

all: build make_iso make_disk run

build: clean drivers libs kernel link

drivers:
	$(MAKE) -C ./src/drivers/

libs:
	mkdir -p output/libs
	$(call build-rule,$(LIBS_C),src/libs,output/libs,.c,.o)
	$(call build-rule,$(LIBS_S),src/libs,output/libs,.asm,.o)

kernel: libs
	mkdir -p output/kernel/
	$(call build-rule,$(KERNEL_C),src/kernel,output/kernel,.c,.o)
	$(call build-rule,$(KERNEL_S),src/kernel,output/kernel,.asm,.o)
	$(MAKE) k_api

k_api:
	mkdir -p output/api/
	$(call build-rule,$(API_C),src/api,output/api,.c,.o)
	$(call build-rule,$(API_S),src/api,output/api,.asm,.o)

link:
	$(LD) $(LDFLAGS) -o output/kernel.elf $(OBJS)

clean:
	rm -rf output/

make_iso:
	cp output/kernel.elf iso/boot/
	grub-mkrescue -o output/os.iso ./iso/

make_disk:
	dd if=/dev/zero of=output/disk.img bs=512 count=20480

run:
	qemu-system-i386 -no-reboot -no-shutdown -monitor stdio \
		-drive file=./output/disk.img,format=raw,if=ide,index=0,media=disk \
		-drive file=output/os.iso,format=raw,if=ide,index=1,media=cdrom \
		-d int,cpu_reset -D qemu.log \
		-boot d
