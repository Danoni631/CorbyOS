/*
    CODED BY Danoni631, on 07/13/2026
    This code get RAM type (DDR4, DDR5, DDR3, DDR2 or anyone) and
    get capacity
*/

#include "ram.h"
#include "boot.h"
#include "graphics.h"

uint32_t get_RAM_capacity(struct multiboot_info* mb_info)
{
    if (mb_info->flags & 0x1)
    {
        return mb_info->mem_lower + mb_info->mem_upper;
    }

    return 0x00;
}

const char* get_RAM_type(VOID)
{
    return "unknown";
}

void init_ram(struct multiboot_info* mb_info, int verbose)
{
    CHAR buff[64];
    uint32_t RAM_KB = get_RAM_capacity(mb_info);
    print("RAM Capacity", 0xFFFFFFFF);
    int lenght = 0;
    uint32_t temp = RAM_KB;
    char num[20];
    
    do
    {
        num[lenght++] = '0' + (temp % 10);
        temp /= 10;
    }

    while (temp > 0);

    for (INT i = lenght - 1; i >= 0; i--)
    {
        buff[lenght - 1 - i] = num[i];
    }

    buff[lenght] = '\0';
    print(buff, 0xFFFFFFFF);
    print("Kilobytes\n", 0xFFFFFFFF);
    print("RAM type: ", 0xFFFF0000);
    print(get_RAM_type(), 0xFFFFFFFF);
}