/*
    CODED BY Danoni631, on 07/10/2026
    Is a timer system
*/

#include "include/stdint.h"
#include "graphics.h"
#include "ports.h"

#include "timer.h"

int ticks = 0;
int seconds = 0; 
int minutes = 0;
int drawBar = FALSE;

void PITIRQ0(struct InterruptRegisters *r)
{
    ticks += 1;

    if (ticks % 20 == 0)
    {
        seconds++;
    }
}

unsigned int get_ticks()
{
    return ticks;
}

void sleep(int seconds)
{
    int startTick = ticks;

    while
    (
        ticks < startTick +
        (
            startTick * (seconds * 10)
        )
    )
    {
        
    }
}

void init_timer(void)
{
    IRQinstall_handler(0x00, &PITIRQ0);

    DWORD divisor = 1193180 / 100;

    outb(0x43, 0x36);
    outb(0x40, (BYTE)(divisor & 0xFFFF));
    outb(0x40, (BYTE)((divisor & 0xFFFF) & 0xFF));
}