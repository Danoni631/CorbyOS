/*
    CODED BY Danoni631, on 07/13/2026

    Shitty implemented drive functions.  Is inaccurate
*/

#include "ports.h"
#include "graphics.h"

#include "disk.h"

#define FLOPPY_SIZE 1474560

void wait_for_ready(WORD base)
{
    BYTE status;

    while (1)
    {
        status = inw(base + 7);

        if (status & IDE_STATUS_READY)
        {
            break;
        }
    }
}

DWORD get_disk_capacity(WORD base)
{
    outw(base + 6, 0xA0);
    outw(base + 7, 0xEC);
    wait_for_ready(base);
    WORD data[256];

    for (INT i = 0; i < 256; i++)
    {
        data[i] = inw(base);
    }

    DWORD sectors =
    (
        (DWORD)data[61] << 16
    ) | data[60];

    return sectors;
}

DWORD get_floppy_capacity(VOID)
{
    BYTE status;
    status = inw(FLOPPY_STATUS_PORT);

    if (status & FLOPPY_STATUS_PORT)
    {
        return FLOPPY_SIZE;
    }
}

void list_all_disk(VOID)
{
    DWORD disk_capacity = get_disk_capacity(IDE_PRIMARY_COMMAND_PORT);

    if (disk_capacity > 0)
    {
        debug("Disk 0: ", 0x02);
        print("Size", 0xFFFFFFFF);
        printhex(disk_capacity, 0xFFFFFFFF);
        print("Sectors\n", 0xFFFFFFFF);
    }

    else
    {
        debug("Don't finded any disk!\n", 0x01);
    }

    DWORD floppy_capacity = get_floppy_capacity();

    if (floppy_capacity > 0)
    {
        debug("Floppy drive: ", 0x02);
        print("Size", 0xFFFFFFFF);
        printint(floppy_capacity, 0xFFFFFFFF);
        print("Bytes", 0xFFFFFFFF);
    }

    else
    {
        debug("Don't find any Floppy!\n", 0x01);
    }
}