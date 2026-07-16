[bits 32]
[global load_idt]

load_idt:
    mov eax, [esp + 4]  ; Load the address of the IDT into EAX
    lidt [eax]          ; Load the IDT register with the new IDT
    sti
    ret

%macro ISRNOERRCODE 1
global isr%1

isr%1:
    cli
    push long %1
    jmp isr_common_stub ; Jump to the common ISR handler

%endmacro

%macro IRQ 2
global irq%1

irq%1:
    cli
    push long %2
    jmp irq_common_stub ; Jump to the common IRQ handler

%endmacro

ISRNOERRCODE 0
ISRNOERRCODE 1
ISRNOERRCODE 2
ISRNOERRCODE 3
ISRNOERRCODE 4
ISRNOERRCODE 5
ISRNOERRCODE 6
ISRNOERRCODE 7
ISRNOERRCODE 8
ISRNOERRCODE 9
ISRNOERRCODE 10
ISRNOERRCODE 11
ISRNOERRCODE 12
ISRNOERRCODE 13
ISRNOERRCODE 14
ISRNOERRCODE 15
ISRNOERRCODE 16
ISRNOERRCODE 17
ISRNOERRCODE 18
ISRNOERRCODE 19
ISRNOERRCODE 20
ISRNOERRCODE 21
ISRNOERRCODE 22
ISRNOERRCODE 23
ISRNOERRCODE 24
ISRNOERRCODE 25
ISRNOERRCODE 26
ISRNOERRCODE 27
ISRNOERRCODE 28
ISRNOERRCODE 29
ISRNOERRCODE 30
ISRNOERRCODE 31
ISRNOERRCODE 128
ISRNOERRCODE 177

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

[extern isr_handler]

isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler

    pop gs
    pop fs
    pop es
    pop ds
    popa

    add esp, 4 ; Clean up the stack (remove the interrupt number)
    iret

[extern irq_handler]

irq_common_stub:
    PUSHA

    mov eax, ds
    
    push eax
    
    mov eax, cr2
    
    push eax
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp
    
    call irq_handler
    
    add esp, 0x08
    
    pop ebx
    
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    
    popa
    add esp, 0x08
    
    sti

    iret

section .note.GNU-stack noalloc noexec nowrite progbits