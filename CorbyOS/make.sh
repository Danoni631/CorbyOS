#!/bin/sh
set -e

ROOT="."
BIN="$ROOT/binaries"
OSROOT="$ROOT/CorbyOS"

mkdir -p "$BIN/corby_ui" "$ROOT/kernel/programs/binaries"

# BOOTLOADER FILE
nasm -f elf32 "$ROOT/kernel/boot.s" -o "$BIN/boot.o"

# KERNEL FILES
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/kernel.c" -o "$BIN/kernel.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/crash.c" -o "$BIN/crash.o"

# GDT FILES
nasm -f elf32 "$ROOT/kernel/gdt.asm" -o "$BIN/gdt_asm.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/gdt.c" -o "$BIN/gdt_c.o"

# IDT FILES
nasm -f elf32 "$ROOT/kernel/idt.asm" -o "$BIN/idt_asm.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/idt.c" -o "$BIN/idt_c.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/syscall.c" -o "$BIN/syscall.o"

# INCLUDE FILES
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/ports.c" -o "$BIN/ports.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/include/math.c" -o "$BIN/math.o"

# GRAPHICS SYSTEM FILES
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/graphics.c" -o "$BIN/graphics.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/print.c" -o "$BIN/print.o"

# FILE SYSTEM FILES
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/fs.c" -o "$BIN/fs.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/programs.c" -o "$BIN/programs.o"

# DRIVERS FILES
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/apm.c" -o "$BIN/apm.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/drive.c" -o "$BIN/drive.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/mouse.c" -o "$BIN/mouse.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/kb.c" -o "$BIN/kb.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/audio.c" -o "$BIN/audio.o"

# TIMER FILE
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/timer.c" -o "$BIN/timer.o"

# HARDWARE FILES
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/cpu.c" -o "$BIN/cpu.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/cmos.c" -o "$BIN/cmos.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/pci.c" -o "$BIN/pci.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/ram.c" -o "$BIN/ram.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/gpu.c" -o "$BIN/gpu.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/disk.c" -o "$BIN/disk.o"

# ELF FILE
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/elf.c" -o "$BIN/elf.o"

# MEMORY FILES
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/mem.c" -o "$BIN/mem.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/vmm.c" -o "$BIN/vmm.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/alloc.c" -o "$BIN/alloc.o"

# SHELL FILES
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/shell.c" -o "$BIN/shell.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/shell_extras.c" -o "$BIN/shell_extras.o"

# CORBY UI FILES
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/corby_ui/login_screen.c" -o "$BIN/corby_ui/login_screen.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/corby_ui/winsys.c" -o "$BIN/corby_ui/winsys.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/corby_ui/gui.c" -o "$BIN/corby_ui/gui.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/corby_ui/userspace.c" -o "$BIN/corby_ui/userspace.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/corby_ui/desktop.c" -o "$BIN/corby_ui/desktop.o"

# PROGRAMS
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/programs/bytebeat.c" -o "$ROOT/kernel/programs/binaries/bytebeat.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/programs/scrolling_text.c" -o "$ROOT/kernel/programs/binaries/scrolling_text.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/programs/cube.c" -o "$ROOT/kernel/programs/binaries/cube.o"
gcc -ffreestanding -m32 -g -c "$ROOT/kernel/programs/kb_test.c" -o "$ROOT/kernel/programs/binaries/kb_test.o"

# ELF PROGRAMS
ld -m i386pe -T "$ROOT/kernel/linker.ld" -Ttext 0xC90000 "$ROOT/kernel/programs/binaries/bytebeat.o" "$BIN/ports.o" "$BIN/audio.o" -o "$ROOT/kernel/programs/elf/bytebeat.elf"
ld -m i386pe -T "$ROOT/kernel/linker.ld" -Ttext 0xC90000 "$ROOT/kernel/programs/binaries/scrolling_text.o" "$BIN/math.o" "$BIN/print.o" "$BIN/graphics.o" -o "$ROOT/kernel/programs/elf/scrolling_text.elf"

# KERNEL LINK
ld -m i386pe -T "$ROOT/kernel/linker.ld" -Ttext 0xC90000 \
    "$BIN/boot.o" \
    "$BIN/alloc.o" \
    "$BIN/apm.o" \
    "$BIN/audio.o" \
    "$BIN/cmos.o" \
    "$BIN/crash.o" \
    "$BIN/corby_ui/desktop.o" \
    "$BIN/corby_ui/gui.o" \
    "$BIN/corby_ui/login_screen.o" \
    "$BIN/corby_ui/userspace.o" \
    "$BIN/corby_ui/winsys.o" \
    "$BIN/disk.o" \
    "$BIN/drive.o" \
    "$BIN/elf.o" \
    "$BIN/fs.o" \
    "$BIN/gdt_asm.o" \
    "$BIN/gdt_c.o" \
    "$BIN/gpu.o" \
    "$BIN/graphics.o" \
    "$BIN/idt_asm.o" \
    "$BIN/idt_c.o" \
    "$BIN/kb.o" \
    "$BIN/kernel.o" \
    "$BIN/mem.o" \
    "$BIN/mouse.o" \
    "$BIN/pci.o" \
    "$BIN/ports.o" \
    "$BIN/print.o" \
    "$BIN/programs.o" \
    "$BIN/ram.o" \
    "$BIN/shell.o" \
    "$BIN/shell_extras.o" \
    "$BIN/syscall.o" \
    "$BIN/timer.o" \
    "$BIN/vmm.o" \
    -o fullkernel

mv fullkernel "$OSROOT/boot" -o CorbyOS/binaries/kernel.bin