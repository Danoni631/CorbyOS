#ifndef GDT_H
#define GDT_H

#include "include/stdint.h"

struct gdt_entry_struct{
    WORD limit;
    WORD base_low;
    BYTE base_middle;
    BYTE access;
    BYTE flags;
    BYTE base_high;
}__attribute__((packed));

struct gdt_ptr_struct{
    WORD limit;
    DWORD base;
}__attribute__((packed));

struct tss_entry_struct{
	DWORD prev_tss;
	DWORD esp0;
	DWORD ss0;
	DWORD esp1;
	DWORD ss1;
	DWORD esp2;
	DWORD ss2;
	DWORD cr3;
	DWORD eip;
	DWORD eflags;
	DWORD eax;
	DWORD ecx;
	DWORD edx;
	DWORD ebx;
	DWORD esp;
	DWORD ebp;
	DWORD esi;
	DWORD edi;
	DWORD es;
	DWORD cs;
	DWORD ss;
	DWORD ds;
	DWORD fs;
	DWORD gs;
	DWORD ldt;
	DWORD trap;
	DWORD iomap_base;
} __attribute__((packed));

void start_gdt();
void set_all_gates();
void setgate(DWORD num, DWORD base, DWORD limit, BYTE access, BYTE gran);
void writetss(DWORD num, WORD ss0, DWORD esp0);

#endif