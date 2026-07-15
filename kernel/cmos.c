/*
    CODED BY Danoni631, on 07/12/2026... ON 01:30 AM

    The CMOS (Complementary Metal-Oxide Semiconductor) is used to store
    basic informations about the system

    MEMORY
    TIME
    DATE
*/

#include "ports.h"
#include "graphics.h"

#include "cmos.h"

USHORT get_cmos_mem()
{
    outb(0x70, 0x30);
    BYTE low = inb(0x71);

    outb(0x70, 0x31);
    BYTE high = inb(0x71);

    USHORT total = low | high << 8;

    return total;
}

void get_cmos_date()
{
    outb(0x70, 0x0A);
    while(inb(0x71) & 0x80);

    outb(0x70, 0x07);
    BYTE Day = inb(0x71);

    outb(0x70, 0x08);
    BYTE Month = inb(0x71);

    outb(0x70, 0x09);
    BYTE Year = inb(0x71);

    Day   = ((Day   / 16) * 10) + (Day   & 0x0F);
    Month = ((Month / 16) * 10) + (Month & 0x0F);
    Year  = ((Year  / 16) * 10) + (Year  & 0x0F);

    printint(Month, 0xFFFFFFFF);
    print("/", 0xFFFFFFFF);
    printint(Day, 0xFFFFFFFF);
    print("/", 0xFFFFFFFF);
    printint(Year, 0xFFFFFFFF);
}

void show_cmos_mem()
{
    debug("CMOS Memory Size: ", 0x02);
    printhex(get_cmos_mem(), 0xFFFFFFFF);
    print("\n", 0x00);
}