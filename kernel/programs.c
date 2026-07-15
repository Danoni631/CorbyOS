#include "include/stdint.h"
#include "elf.h"
#include "alloc.h"
#include "fs.h"
#include "graphics.h"

#include "programs.h"

void run_program(char* filename)
{
    for (int i = 0; filename[i]; i++)
    {
        if (filename[i] == '\n' || filename[i] == '\r')
        {
            debug("Comparing with: [", 0x02);
            print(currentDir->files[i].filename, 0xFF00FFFF);
            print("]\n", 0xFF00FFFF);
            
            filename[i] = '\0';
            break;
        }
    }

    DWORD size;
    LPBYTE buffer = alloc_memory(1024 * 1024);

    print("\n", 0x00);
    debug("Trying to open: [", 0x02);
    print(filename, 0xFFFFFFFF);
    print("]\n", 0xFFFFFFFF);

    if (read_file(filename, buffer, &size) != 0)
    {
        debug("Searching for: [", 0x02);
        print(filename, 0xFFFFFF00);
        print("]\n", 0xFFFFFF00);
    }

    debug("ELF Magic: ", 0x02);
    printhex(*(DWORD*)buffer, 0xFFFFFFFF);
    print("\n", 0x00);

    debug("File loaded!\n", 0x02);

    VOID (*entry)() = (void (*)()) start_elf(buffer, 1);

    if (!entry)
    {
        debug("Invalid ELF!\n", 0x01);
        return;
    }

    debug("Entry: ", 0x02);
    printhex((DWORD)entry, 0xFFFFFFFF);
    print("\n", 0x00);

    debug("Executing...\n", 0x02);

    entry();
}