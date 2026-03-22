CC = i386-elf-gcc
CFLAGS = -w -ffreestanding -m32 -fno-pie -nostdlib
NS = nasm
NSFLAGS = -f elf32

# Собираем списки исходников
LIBS_C    := $(shell find src/libs -name "*.c")
LIBS_S    := $(shell find src/libs -name "*.asm")
KERNEL_C  := $(shell find src/kernel -name "*.c")
KERNEL_S  := $(shell find src/kernel -name "*.asm")
API_C     := $(shell find src/api -name "*.c")
API_S     := $(shell find src/api -name "*.asm")

all: build make_iso make_disk run

# Правило для компиляции всего
build: clean
	@echo "\n--- Building Drivers ---"
	$(MAKE) -C ./src/drivers/
	@echo "\n--- Building Libs ---"
	$(MAKE) compile_libs
	@echo "\n--- Building Kernel ---"
	$(MAKE) compile_kernel
	@echo "\n--- Building API ---"
	$(MAKE) compile_api
	@echo "\n--- Linking ---"
	$(MAKE) link

compile_libs:
	mkdir -p output/libs
	for f in $(LIBS_C); do $(CC) $(CFLAGS) -c $$f -o output/libs/$$(basename $$f .c).o; done
	for f in $(LIBS_S); do $(NS) $(NSFLAGS) $$f -o output/libs/$$(basename $$f .asm).o; done

compile_kernel:
	mkdir -p output/kernel
	# 1. ПРИНУДИТЕЛЬНО КОМПИЛИРУЕМ ГЛАВНЫЙ ФАЙЛ
	$(CC) $(CFLAGS) -c src/kernel/kernel.c -o output/kernel/kernel.o
	# 2. Компилируем все остальные .c файлы в подпапках
	for f in $$(find src/kernel -name "*.c" | grep -v "kernel.c"); do \
		name=$$(echo $$f | sed 's|src/kernel/||' | tr '/' '_'); \
		$(CC) $(CFLAGS) -c $$f -o output/kernel/$$name.o; \
	done
	# 3. Компилируем все .asm файлы
	for f in $$(find src/kernel -name "*.asm"); do \
		name=$$(echo $$f | sed 's|src/kernel/||' | tr '/' '_'); \
		$(NS) $(NSFLAGS) $$f -o output/kernel/$$name.o; \
	done

compile_api:
	mkdir -p output/api
	for f in $(API_C); do $(CC) $(CFLAGS) -c $$f -o output/api/$$(basename $$f .c).o; done
	for f in $(API_S); do $(NS) $(NSFLAGS) $$f -o output/api/$$(basename $$f .asm).o; done

link:
	# Собираем все .o файлы, которые нагенерили
	$(eval CURRENT_OBJS := $(shell find output -name "*.o"))
	i386-elf-ld -m elf_i386 -T src/linker.ld --oformat elf32-i386 -o output/kernel.elf $(CURRENT_OBJS)

make_iso:
	cp output/kernel.elf iso/boot/
	grub-mkrescue -o output/os.iso ./iso/

make_disk:
	mkdir -p output
	dd if=/dev/zero of=output/disk.img bs=512 count=20480

run:
	qemu-system-i386 -no-reboot -no-shutdown -monitor stdio \
	-drive file=output/disk.img,format=raw,if=ide,index=0,media=disk \
	-drive file=output/os.iso,format=raw,if=ide,index=1,media=cdrom \
	-boot d

clean:
	rm -rf output/

