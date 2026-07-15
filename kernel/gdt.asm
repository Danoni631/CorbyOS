global gdt_flush

gdt_flush:
    mov eax, [esp + 4]  ; Load the address of the GDT into EAX
    lgdt [eax]          ; Load the GDT register with the new GDT
    
    mov eax, 0x10
    mov ds, ax  ; DATA SEGMENT
    mov es, ax  ; EXTRA SEGMENT
    mov fs, ax  ; FS SEGMENT
    mov gs, ax  ; GS SEGMENT
    mov ss, ax  ; STACK SEGMENT

    jmp 0x08:flush_done  ; Jump to the code segment selector (0x08) and flush_done label

flush_done:
    ret

global tss_flush

tss_flush:
    mov eax, [esp + 4]  ; Load the address of the TSS into EAX
    ltr ax              ; Load the Task Register with the new TSS selector
    ret