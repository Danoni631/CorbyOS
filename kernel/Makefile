CC      = gcc # GCC is the C compilator
ASM     = nasm # NASM is the x86 ASM compilator
LD      = ld # LD is a runner of Linkerscript files (linking C with ASM)

CFLAGS  = -ffreestanding -m32 -g
ASMFLAGS= -felf32
LDFLAGS = -m elf_i386 -T CorbyOS/kernel/linker.ld

BIN     = CorbyOS/binaries

C_Sources := $(shell find . -name "*.c")

ASM_Sources := $(shell find . -name "*.asm" \
			-not -path "CorbyOS/CorbyOS/*")

S_SRCS := $(shell find . -name "*.s")

C_OBJS := $(patsubst ./%.c,$(BIN)/%.o,$(C_SRCS))
ASM_OBJS := $(patsubst ./%.asm,$(BIN)/%.o,$(ASM_SRCS))
S_OBJS := $(patsubst ./%.s,$(BIN)/%.o,$(S_SRCS))

OBJS := $(C_OBJS) $(ASM_OBJS) $(S_OBJS)

BOOT_OBJ = CorbyOS/binaries/boot.o

S_OBJS := $(filter-out $(BIN)CorbyOS/binaries/boot.o,$(S_OBJS))

OBJS := $(C_OBJS) $(ASM_OBJS) $(S_OBJS)

# ------------------------------------------------------

all: kernel iso

remakeboot:
	rm -f boot.o
	nasm -felf32 Bootloader/boot.s -o Bootloader/boot.o

# ------------------------------------------------------

$(BIN)/%.o: %.c
	@mkdir -p $(dir $@)
	@printf "\033[1;34m[CC]\033[0m %s\n" "$<"
	$(CC) $(CFLAGS) -c $< -o $@
+
# ------------------------------------------------------

$(BIN)/%.o: %.asm
	@mkdir -p $(dir $@)
	@printf "\033[1;36m[ASM]\033[0m %s\n" "$<"
	$(ASM) $(ASMFLAGS) $< -o $@

# ------------------------------------------------------

$(BOOT_OBJ): CorbyOS/kernel/boot.s
	@printf "\033[1;32m[BOOT]\033[0m %s\n" "$<"
	$(ASM) $(ASMFLAGS) $< -o $@

$(BIN)/%.o: %.s
	@mkdir -p $(dir $@)
	@printf "\033[1;36m[ASM]\033[0m %s\n" "$<"
	$(ASM) $(ASMFLAGS) $< -o $@

# ------------------------------------------------------

kernel: $(BOOT_OBJ) $(GEARS_OBJ) $(OBJS)
	@printf "\033[1;33m[Linking]\033[0m\n"
	@$(LD) $(LDFLAGS) -o fullkernel \
		$(BOOT_OBJ) \
		$(OBJS) \
		$(GEARS_OBJ)

	mv fullkernel CorbyOS/CorbyOS/boot

# ------------------------------------------------------

iso: kernel
	grub-mkrescue -o CorbyOS_1.0.iso CorbyOS/CorbyOS/

# ------------------------------------------------------

run: iso
	qemu-system-x86_64 \
		-device sb16 \
		-netdev user,id=net0 \
		-net nic,model=rtl8139,netdev=net0 \
		-object filter-dump,id=f1,netdev=net0,file=net.pcap \
		-serial stdio \
		-drive file=NovaOS.iso,format=raw,if=ide,index=0 \
		-drive file=disk.raw,format=raw,if=ide,index=2

# ------------------------------------------------------

clean:
	rm -rf CorbyOS/binaries
	rm -f boot.o
	rm -f kernel.bin
	rm -f CorbyOS_1.0.iso

.PHONY: all kernel iso run clean