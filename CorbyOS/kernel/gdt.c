#include "mem.h"
#include "gdt.h"

extern VOID gdt_flush(uint32_t);
extern VOID tss_flush();

struct gdt_entry_struct gdt_entries[6];
struct gdt_ptr_struct gdt_ptr;
struct tss_entry_struct tss_entry;

void start_gdt()
{
    gdt_ptr.limit =
    (
        sizeof
        (
            struct gdt_entry_struct
        ) * 6
    ) - 1;

    gdt_ptr.base = (uint32_t)&gdt_entries;

    set_all_gates();

    writetss(5, 0x10, 0x0);
    gdt_flush
    (
        (
            uint32_t
        )&gdt_ptr
    );

    tss_flush();
}

void set_all_gates()
{
    setgate(0, 0, 0, 0, 0);
    setgate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    setgate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    setgate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    setgate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
}

void writetss(DWORD num, WORD ss0, DWORD esp0)
{
    uint32_t base = (uint32_t) &tss_entry;
    uint32_t limit = base + sizeof(tss_entry);

    setgate(num, base, limit, 0xE9, 0x00);
    memset(&tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0 = ss0;
    tss_entry.esp0 = esp0;

    tss_entry.cs = 0x08 | 0x3;
    tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x10 | 0x3;
}

void setgate(DWORD num, DWORD base, DWORD limit, BYTE access, BYTE gran)
{
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit = (limit & 0xFFFF);
    gdt_entries[num].flags = (limit >> 16) & 0x0F;
    gdt_entries[num].flags |= (gran & 0xF0);

    gdt_entries[num].access = access;
}