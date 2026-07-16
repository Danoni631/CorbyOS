/*
    CODED BY Danoni631, in 07/04/2026
    The kernel starts drivers, hardware, graphics, tables and other.
*/

#include "include/stdint.h"
#include "boot.h"
#include "graphics.h"
#include "idt.h"
#include "timer.h"
#include "vmm.h"
#include "kb.h"
#include "mouse.h"
#include "drive.h"
#include "audio.h"
#include "cpu.h"
#include "gpu.h"
#include "pci.h"
#include "ram.h"
#include "cmos.h"
#include "disk.h"
#include "fs.h"
#include "shell.h"
#include "corby_ui/winsys.h"
#include "corby_ui/gui.h"
#include "corby_ui/userspace.h"
#include "gdt.h"

static DWORD octaltoint(char* str)
{
    DWORD value = 0;

    while (*str >= '0' && *str <= '7')
    {
        value = (value * 8) + (*str - '0');
        str++;
    }

    return value;
}

void main(struct multiboot_info* mbinfo, DWORD addr)
{
    struct vbe_mode_info_t* vbe = 
    (
        struct vbe_mode_info_t*
    ) mbinfo->vbe_mode_info;
    
    init_graphics(vbe->framebuffer, vbe->pitch);    //Start Graphics
    debug("VESA 1920x1080 32BPP Started\n", 0x00);
    debug("Framebuff: ", 0x00);
    printhex(get_frame_buff(), 0xFFFFFFFF);     //Get frame buffer
    print("\n", 0x00);
    debug("Pitch: ", 0x00);
    printhex(get_pitch(), 0xFFFFFFFF);
    print("\n", 0x00);

    if (addr != 0x2BADB002)
    {
        debug("Invalid magic number\n", 0x01);
        return;
    }

    debug("Valid magic number!: 0x2BADB002\n", 0x00);
    
    for (int i = 0; i < mbinfo->mmap_length; i += sizeof(struct multiboot_mmap_entry))
    {
        struct multiboot_mmap_entry* entry = (struct multiboot_mmap_entry*)(mbinfo->mmap_addr + i);
        debug(" Low addr: ", 0x02);
        printhex(entry->addr_low, 0xFFFFFFFF);
        print(" High Addr: ", 0xFFFFFFFF);
        printhex(entry->addr_high, 0xFFFFFFF);
        print(" Low length: ", 0xFFFFFFFF);
        printhex(entry->len_low, 0xFFFFFFFF);
        print(" High length: ", 0xFFFFFFFF);
        printhex(entry->len_high, 0xFFFFFFFF);
        print("  Size: ", 0xFFFFFFFF);
        printint(entry->size, 0xFFFFFFFF);
        print(" Type: ", 0xFFFFFFFF);
        printint(entry->type, 0xFFFFFFFF);
        print("\n", 0x00);
    }

    debug("Kernel loaded\n", 0x00);
    start_gdt();
    debug("GDT Started!\n", 0x00);
    debug("TSS Started\n", 0x00);
    setup_idt();
    debug("IDT Started\n", 0x00);
    init_virt_mem();
    debug("Virtual Memory Manager loaded\n", 0x00);
    
    DWORD aligned = 
    (
        (
            mbinfo->mods_addr + 3
        ) & ~3
    );

    struct multiboot_module_t* mods =
    (
        (
            struct multiboot_module_t*
        )
    
        mbinfo->mods_addr
    );

    init_filesystem();
    make_folder("system");

    for (int i = 0; i < mbinfo->mods_count; i++)
    {
        void* start = (void*) mods[i].mod_start;
        void* end   = (void*) mods[i].mod_end;
        DWORD size = (DWORD)((BYTE*)end - (BYTE*)start);

        char* filename = get_file_data((char*)mods[i].string);
        
        make_file(filename, (LPBYTE)start, size, PERM_R | PERM_W | PERM_X);
    }

    make_folder("home");
    debug("FileSystem loaded\n", 0x00);
    init_timer();
    debug("Timer loaded\n", 0x00);
    sleep(10);
    init_keyboard();
    init_mouse();
    setup_sb();
    debug("Drivers started\n", 0x00);
    show_cmos_mem();
    list_all_disks();
    show_cpu_info();
    show_gpu_info();
    show_pci_devices();
    init_ram(mbinfo, 1);
    debug("Hardware items started\n", 0x00);
    sleep(2);
    clsscreen();
    debug("Your system was loading...", 0x00);
    sleep(1000);
    clsscreen();
    draw_boot_screen();
    clsscreen();
    start_shell_no_gui();
    start_userspace();
}