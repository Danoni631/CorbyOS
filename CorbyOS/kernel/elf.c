/*
    CODED BY Danoni631, on 07/14/2026
    It loads ELF32 files (Only static elfs)
*/

#include "graphics.h"
#include "mem.h"
#include "vmm.h"
#include "fs.h"
#include "corby_ui/winsys.h"
#include "corby_ui/userspace.h"

#include "elf.h"

INT start_elf(VOID* elfData, INT run)
{
    DWORD relocationOffs = 0;

    if (run)
    {
        print("\n", 0x00);
        debug("Loading ELF...\n", 0x02);
    }

    ELF32_Header* elfHeader = (ELF32_Header*)elfData;
    
    if (*(DWORD*)elfHeader->e_ident != ELFMAGIC) 
    {
        debug("Invalid ELF File\n", 0x01);
        return -1;
    }

    if (run) debug("Valid ELF File!\n", 0x02);

    if (elfHeader->e_machine != ELFARCH) 
    {
        debug("Non 32-bits program!\n", 0x01);
        return -1;
    }

    if (run) debug("Valid ELF Architecture!\n", 0x02);

    ELF32_ProgramHeader* programHeader =
    (
        ELF32_ProgramHeader*
    )
        (
            (BYTE*)elfData + elfHeader->e_phoff
        );

    for (WORD i = 0; i < elfHeader->e_phnum; i++) 
    {
        if (programHeader[i].p_type != PTLOAD) 
        {
            continue;
        }

        char* dest = (char*) programHeader[i].p_vaddr;
        char* src = elfData + programHeader[i].p_offset;
        DWORD filesz = programHeader[i].p_filesz;
        DWORD memsz = programHeader[i].p_memsz;

        allocate_virt_mem
        (
            (DWORD)
            dest,
            memsz,
            PROT_READ | PROT_WRITE | PROT_EXEC,
            1
        );

        if (filesz > 0) 
        {
            if (run)
            {
                debug("Loading segment\n", 0x02);
                debug("Segment: ", 0x02);
                printhex(programHeader[i].p_vaddr, 0xFFFFFFFF);
                print(" - ", 0xFFFFFFFF);
                
                printhex
                (
                    programHeader[i].p_vaddr +
                    programHeader[i].p_memsz,
                    0xFFFFFFFF
                );

                print("\n", 0x00);
            }

            memmove(dest, src, filesz);
        }

        if (memsz > filesz) 
        {
            if (run) debug("Zeroing BSS\n", 0x02);
            memset(dest + filesz, 0, memsz - filesz);
        }
    }
    DWORD relocatedEntry = elfHeader->e_entry + relocationOffs;

    if (run)
    {
        debug("ELF Entry Point: ", 0x02);
        printhex(relocatedEntry, 0xFFFFFFFF);
        print("\n", 0x00);
    }

    int argc = 3;
    char* argv[] = { "doomgeneric", "-iwad", "doom1.wad", 0x00 };
    void (*entryPoint)(int, char**);
    entryPoint = (void(*)(int, char**))(relocatedEntry);
    entryPoint(argc, argv);
    return 0;
}

VOID run_elf(VOID* elf)
{
    if (start_elf(elf, 1))
    {
        debug("Failed to Load Program\n", 0x01);
        return;
    }
}