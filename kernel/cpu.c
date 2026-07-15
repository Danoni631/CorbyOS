#include "include/stdint.h"
#include "graphics.h"

#include "cpu.h"

CHAR Vendor[13];
CHAR Name[49];

void get_cpu_vendor(char* vendor)
{
    INT ebx;
    INT edx;
    INT ecx;

    asm volatile
    (
        "xor %%eax, %%eax\n\t"
        "cpuid\n\t"
        : "=b" (ebx),
          "=d" (edx),
          "=c" (ecx)
        :
        : "eax"
    );

    *(int *)&vendor[0] = ebx;
    *(int *)&vendor[4] = edx;
    *(int *)&vendor[8] = ecx;
    vendor[12] = 0;
}

void get_cpu_name(char* name)
{
    DWORD eax;
    DWORD ebx;
    DWORD ecx;
    DWORD edx;

    for (INT i = 0; i < 3; i++)
    {
        eax = 0x80000002 + i;

        asm volatile
        (
            "cpuid"
            : "=a" (eax),
              "=b" (ebx),
              "=c" (ecx),
              "=d" (edx)
            : "a" (eax)
        );

        *(unsigned int*)(name + i * 16 + 0) = eax;
        *(unsigned int*)(name + i * 16 + 4) = ebx;
        *(unsigned int*)(name + i * 16 + 8) = ecx;
        *(unsigned int*)(name + i * 16 + 12) = edx;
    }

    name[48] = '\0';
}

void show_cpu_info()
{
    get_cpu_name(Name);
    get_cpu_vendor(Vendor);
    debug("CPU Vendor: ", 2);
    print(Vendor, 0xFFFFFFFF);
    print("\n", 0x00);
    debug("CPU Name: ", 2);
    print(Name, 0xFFFFFFFF);
    print("\n", 0x00);
}

void show_cpu_name()
{
    print(Name, 0xFFFFFFFF);
    print("\n", 0x00);
}