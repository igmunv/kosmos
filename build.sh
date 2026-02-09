#!/usr/bin/env bash

# Simple builder to replace the old Makefile.
# Usage: ./build.sh [all|build|clean|drivers|libs|kernel|link|iso|disk|run]

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT_DIR"

CC="${CC:-$(command -v i386-elf-gcc || command -v gcc || true)}"
LD="${LD:-$(command -v i386-elf-ld || command -v ld || true)}"
NASM_BIN="${NASM:-$(command -v nasm || true)}"

if [[ -z "$CC" ]]; then
  echo "Compiler not found (install i386-elf-gcc or gcc)"; exit 1
fi
if [[ -z "$LD" ]]; then
  echo "Linker not found (install i386-elf-ld or ld)"; exit 1
fi
if [[ -z "$NASM_BIN" ]]; then
  echo "nasm assembler not found"; exit 1
fi

CFLAGS="${CFLAGS:--w -ffreestanding -m32 -fno-pie -nostdlib -fno-stack-protector -fno-stack-check -std=gnu99}"
NSFLAGS="${NSFLAGS:--f elf32}"

build_rule() {
  local src_list=("$@")
  for src in "${src_list[@]}"; do
    local obj="${src/src/output}"
    obj="${obj%.*}.o"
    mkdir -p "$(dirname "$obj")"
    echo "Building $src -> $obj"
    case "$src" in
      *.c)   "$CC" $CFLAGS -c "$src" -o "$obj" ;;
      *.asm) "$NASM_BIN" $NSFLAGS "$src" -o "$obj" ;;
    esac
  done
}

clean() {
  rm -rf output/
  rm -f iso/boot/kernel.elf
}

build_drivers() {
  mkdir -p output/drivers
  mapfile -t drivers_src < <(find src/drivers -type f \( -name '*.c' -o -name '*.asm' \))
  build_rule "${drivers_src[@]}"
}

build_libs() {
  mkdir -p output/libs
  mapfile -t libs_src < <(find src/libs -type f \( -name '*.c' -o -name '*.asm' \))
  build_rule "${libs_src[@]}"
}

build_kernel() {
  mkdir -p output/kernel output/api
  mapfile -t ksrc < <(find src/kernel -type f \( -name '*.c' -o -name '*.asm' \))
  mapfile -t apisrc < <(find src/api -type f \( -name '*.c' -o -name '*.asm' \))
  build_rule "${ksrc[@]}" "${apisrc[@]}"
}

link_kernel() {
  mapfile -t objs < <(find output -name '*.o')
  "$LD" -m elf_i386 -T src/linker.ld --oformat elf32-i386 -o output/kernel.elf "${objs[@]}"
}

make_iso() {
  mkdir -p output iso/boot
  cp output/kernel.elf iso/boot/
  grub-mkrescue -o output/os.iso ./iso/
}

make_disk() {
  mkdir -p output
  dd if=/dev/zero of=output/disk.img bs=512 count=20480 status=none
}

run_qemu() {
  qemu-system-i386 \
    -drive file=output/disk.img,format=raw,if=ide,index=0,media=disk \
    -drive file=output/os.iso,format=raw,if=ide,index=1,media=cdrom \
    -boot d "$@"
}

case "${1:-all}" in
  clean) clean ;;
  drivers) clean; build_drivers ;;
  libs) clean; build_drivers; build_libs ;;
  kernel) clean; build_drivers; build_libs; build_kernel ;;
  link) clean; build_drivers; build_libs; build_kernel; link_kernel ;;
  iso) clean; build_drivers; build_libs; build_kernel; link_kernel; make_iso ;;
  disk) clean; build_drivers; build_libs; build_kernel; link_kernel; make_disk ;;
  run) clean; build_drivers; build_libs; build_kernel; link_kernel; make_iso; make_disk; shift; run_qemu "$@" ;;
  build|all) clean; build_drivers; build_libs; build_kernel; link_kernel; make_iso; make_disk ;;
  *) echo "Unknown target '$1'"; exit 1 ;;
esac
