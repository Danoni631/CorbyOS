/*
    CODED BY mell-o-tron, on 12/20/2020
    i copied this on MellOS, and syscall made by ArTic(a.k.a JhoPro)
*/

#include "ports.h"
#include "mem.h"
#include "graphics.h"
#include "crash.h"

#include "idt.h"
#include "syscall.h"

static IRQhandler_t IRQRoutines[IRQ_COUNT][IRQ_MAX_HANDLERS];
static int IRQHandlerCount[IRQ_COUNT];

extern VOID load_idt(DWORD);

struct IDT_GateDescriptor idt[TOTALGATES];
struct IDT_Descriptor idtr;

void setupidtgate(BYTE entry, DWORD isr, WORD selector, BYTE flags)
{
    idt[entry].selector = selector;
    idt[entry].reserved = 0x00;
    idt[entry].flags = flags;

    idt[entry].offset  = isr & 0xFFFF;
    idt[entry].offset2 = (isr >> 16) & 0xFFFF;
}

void setup_idt(VOID)
{
    int IDTSize = sizeof(struct IDT_GateDescriptor) * TOTALGATES;

    idtr.size   = IDTSize - 1;
    idtr.offset = (DWORD) &idt;

    memset(&idt, 0, IDTSize);

    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x00);
    outb(0xA1, 0x00);

    setupidtgate(0, (DWORD)isr0, 0x08, 0x0E);
    setupidtgate(1, (DWORD)isr1,0x08, 0x8E);
    setupidtgate(2, (DWORD)isr2,0x08, 0x8E);
    setupidtgate(3, (DWORD)isr3,0x08, 0x8E);
    setupidtgate(4, (DWORD)isr4, 0x08, 0x8E);
    setupidtgate(5, (DWORD)isr5, 0x08, 0x8E);
    setupidtgate(6, (DWORD)isr6, 0x08, 0x8E);
    setupidtgate(7, (DWORD)isr7, 0x08, 0x8E);
    setupidtgate(8, (DWORD)isr8, 0x08, 0x8E);
    setupidtgate(9, (DWORD)isr9, 0x08, 0x8E);
    setupidtgate(10, (DWORD)isr10, 0x08, 0x0E);
    setupidtgate(11, (DWORD)isr11,0x08, 0x8E);
    setupidtgate(12, (DWORD)isr12,0x08, 0x8E);
    setupidtgate(13, (DWORD)isr13,0x08, 0x8E);
    setupidtgate(14, (DWORD)isr14, 0x08, 0x8E);
    setupidtgate(15, (DWORD)isr15, 0x08, 0x8E);
    setupidtgate(16, (DWORD)isr16, 0x08, 0x8E);
    setupidtgate(17, (DWORD)isr17, 0x08, 0x8E);
    setupidtgate(18, (DWORD)isr18, 0x08, 0x8E);
    setupidtgate(19, (DWORD)isr19, 0x08, 0x8E);
    setupidtgate(20, (DWORD)isr20, 0x08, 0x0E);
    setupidtgate(21, (DWORD)isr21,0x08, 0x8E);
    setupidtgate(22, (DWORD)isr22,0x08, 0x8E);
    setupidtgate(23, (DWORD)isr23,0x08, 0x8E);
    setupidtgate(24, (DWORD)isr24, 0x08, 0x8E);
    setupidtgate(25, (DWORD)isr25, 0x08, 0x8E);
    setupidtgate(26, (DWORD)isr26, 0x08, 0x8E);
    setupidtgate(27, (DWORD)isr27, 0x08, 0x8E);
    setupidtgate(28, (DWORD)isr28, 0x08, 0x8E);
    setupidtgate(29, (DWORD)isr29, 0x08, 0x8E);
    setupidtgate(30, (DWORD)isr30, 0x08, 0x8E);
    setupidtgate(31, (DWORD)isr31, 0x08, 0x8E);

    setupidtgate(32, (DWORD)irq0, 0x08, 0x8E);
    setupidtgate(33, (DWORD)irq1, 0x08, 0x8E);
    setupidtgate(34, (DWORD)irq2, 0x08, 0x8E);
    setupidtgate(35, (DWORD)irq3, 0x08, 0x8E);
    setupidtgate(36, (DWORD)irq4, 0x08, 0x8E);
    setupidtgate(37, (DWORD)irq5, 0x08, 0x8E);
    setupidtgate(38, (DWORD)irq6, 0x08, 0x8E);
    setupidtgate(39, (DWORD)irq7, 0x08, 0x8E);
    setupidtgate(40, (DWORD)irq8, 0x08, 0x8E);
    setupidtgate(41, (DWORD)irq9, 0x08, 0x8E);
    setupidtgate(42, (DWORD)irq10, 0x08, 0x8E);
    setupidtgate(43, (DWORD)irq11, 0x08, 0x8E);
    setupidtgate(44, (DWORD)irq12, 0x08, 0x8E);
    setupidtgate(45, (DWORD)irq13, 0x08, 0x8E);
    setupidtgate(46, (DWORD)irq14, 0x08, 0x8E);
    setupidtgate(47, (DWORD)irq15, 0x08, 0x8E);

    setupidtgate(0x80, (DWORD)isr128, 0x08, 0x8E);
    setupidtgate(177, (DWORD)isr177, 0x08, 0x8E);

    load_idt((DWORD)&idtr);
}

void ISRhandler(struct InterruptRegisters* regs)
{
    if (regs->int_no < 32)
    {
        print("\nEAX: ", 0xFFFFFFFF);
        printhex(regs->eax, 0xFFFFFFFF);
        print("\nEBX: ", 0xFFFFFFFF);
        printhex(regs->ebx, 0xFFFFFFFF);
        print("\nECX: ", 0xFFFFFFFF);
        printhex(regs->ecx, 0xFFFFFFFF);
        print("\nEDX: ", 0xFFFFFFFF);
        printhex(regs->edx, 0xFFFFFFFF);
        print("\nESP: ", 0xFFFFFFFF);
        printhex(regs->esp, 0xFFFFFFFF);
        print("\nEBP: ", 0xFFFFFFFF);
        printhex(regs->ebp, 0xFFFFFFFF);
        print("\nESI: ", 0xFFFFFFFF);
        printhex(regs->esi, 0xFFFFFFFF);
        print("\nEDI: ", 0xFFFFFFFF);
        printhex(regs->edi, 0xFFFFFFFF);

        for
        (
            ;;
        );
    }

    if (regs->int_no == 128)
    {
        syscall_handler(regs);
        return;
    }
}

void IRQ_install_handler
(
    int irq, 
    void
    (
        *handler
    )
    (
        struct InterruptRegisters *r
    )
)
{
    if (irq < 0 || irq >= IRQ_COUNT) return;

    if (IRQHandlerCount[irq] >= IRQ_MAX_HANDLERS) return;

    IRQRoutines[irq][IRQHandlerCount[irq]++] = handler;
}

void IRQuninstall_handler(int irq, IRQhandler_t handler)
{
    if (irq < 0 || irq >= IRQ_COUNT)
        return;

    for (int i = 0; i < IRQHandlerCount[irq]; i++)
    {
        if (IRQRoutines[irq][i] == handler)
        {
            for (int j = i; j < IRQHandlerCount[irq] - 1; j++)
            {
                IRQRoutines[irq][j] = IRQRoutines[irq][j + 1];
            }

            IRQHandlerCount[irq]--;
            break;
        }
    }
}

void irq_handler(struct InterruptRegisters* regs)
{
    int irq = regs->int_no - 32;

    if (irq >= 0 && irq < IRQ_COUNT)
    {
        for (int i = 0; i < IRQHandlerCount[irq]; i++)
        {
            if (IRQRoutines[irq][i]) 
            {
                IRQRoutines[irq][i](regs);
            }
        }
    }

    if (regs->int_no >= 40) outb(0xA0, 0x20);

    outb(0x20, 0x20);
}