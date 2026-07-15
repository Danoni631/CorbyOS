; CODED BY Danoni631 (a.k.a Danilo Sensei), in 06/30/2026 and remaked in 07/01/2026

MBOOT_PAGE_ALIGN  equ 1 << 0
MBOOT_MEMORY_INFO  equ 1 << 1
MBOOT_VIDEO_MODE  equ 1 << 2

MBOOT_MAGIC  equ 0x1BADB002
MBOOT_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEMORY_INFO | MBOOT_VIDEO_MODE
MBOOT_CHECKSUM  equ -(MBOOT_MAGIC + MBOOT_FLAGS)

section .multiboot

align 4
    dd MBOOT_MAGIC
    dd MBOOT_FLAGS
    dd MBOOT_CHECKSUM
    dd 0x00000000

    dd 1
    dd 1920
    dd 1080
    dd 32

align 16
section .text

global _start

_start:
    push eax
    push ebx

    xor ebp, ebp

    extern main
    call main

halt_kernel:
    hlt
    jmp halt_kernel

incbin "CorbyOS/kernel/entry.asm"

section .bss
align 16
section .note.GNU-stack noalloc noexec nowrite progbits