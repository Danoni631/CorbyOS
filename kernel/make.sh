compiling_files:
	# BOOTLOADER FILE
	nasm -f bin "CorbyOS/kernel/boot.s" -o "CorbyOS/binaries/boot.o"

	# KERNEL FILES
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/kernel.c -o CorbyOS/Binaries/kernel.o
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/crash.c -o CorbyOS/Binaries/crash.o

	# GDT FILES
	nasm -f bin "CorbyOS/kernel/gdt.asm" -o "CorbyOS/binaries/gdt_asm.o"
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/gdt.c -o CorbyOS/Binaries/gdt_c.o

	# IDT FILES
	nasm -f bin "CorbyOS/kernel/idt.asm" -o "CorbyOS/binaries/idt_asm.o"
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/idt.c -o CorbyOS/Binaries/idt_c.o
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/syscall.c -o CorbyOS/Binaries/syscall.o

	# INCLUDE FILES
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/ports.c -o CorbyOS/Binaries/ports.o
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/include/math.c -o CorbyOS/Binaries/math.o

	# GRAPHICS SYSTEM FILES
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/graphics.c -o CorbyOS/Binaries/graphics.o
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/print.c -o CorbyOS/Binaries/print.o

    # FILE SYSTEM FILES
    gcc -ffreestanding -m32 -g -c CorbyOS/kernel/fs.c -o CorbyOS/Binaries/fs.o
    gcc -ffreestanding -m32 -g -c CorbyOS/kernel/programs.c -o CorbyOS/Binaries/programs.o

    # DRIVERS FILES
    gcc -ffreestanding -m32 -g -c CorbyOS/kernel/apm.c -o CorbyOS/Binaries/apm.o
    gcc -ffreestanding -m32 -g -c CorbyOS/kernel/drive.c -o CorbyOS/Binaries/drive.o
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/mouse.c -o CorbyOS/Binaries/mouse.o
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/kb.c -o CorbyOS/Binaries/kb.o
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/audio.c -o CorbyOS/Binaries/audio.o

    # TIMER FILE
    gcc -ffreestanding -m32 -g -c CorbyOS/kernel/timer.c -o CorbyOS/Binaries/timer.o

    # HARDWARE FILES
    gcc -ffreestanding -m32 -g -c CorbyOS/kernel/cpu.c -o CorbyOS/Binaries/cpu.o
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/cmos.c -o CorbyOS/Binaries/cmos.o
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/pci.c -o CorbyOS/Binaries/pci.o
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/ram.c -o CorbyOS/Binaries/ram.o
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/gpu.c -o CorbyOS/Binaries/gpu.o
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/disk.c -o CorbyOS/Binaries/disk.o

	# ELF FILE
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/elf.c -o CorbyOS/Binaries/elf.o

	# MEMORY FILES
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/mem.c -o CorbyOS/Binaries/mem.o
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/vmm.c -o CorbyOS/Binaries/vmm.o
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/alloc.c -o CorbyOS/Binaries/alloc.o

	# SHELL FILES
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/shell.c -o CorbyOS/Binaries/shell.o
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/shell_extras.c -o CorbyOS/Binaries/shell_extras.o

	# CORBY UI FILES
	gcc -ffreestanding -m32 -g -c CorbyOS/kernel/corby_ui/login_screen.c -o CorbyOS/Binaries/corby_ui/login_screen.o