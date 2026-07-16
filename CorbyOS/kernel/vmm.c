// CODED BY Danoni631, on 07/14/2026

#include "vmm.h"

VirtualMemoryManager vmm;

void init_virt_mem()
{
    for (int i = 0; i < TOTAL_PAGES; i++)
    {
        vmm.pageTable[i].physicalAddress = 0;
        vmm.pageTable[i].present = 0;
        vmm.pageTable[i].writable = 0;
        vmm.pageTable[i].user = 0;
    }
}

int map_page
(
    DWORD virtualAddress, 
    DWORD physicalAddress, 
    BYTE writable, 
    BYTE user
)
{
    DWORD pageIndex = virtualAddress / PAGE_SIZE;

    if (pageIndex >= TOTAL_PAGES) 
    {
        return -1;
    }

    vmm.pageTable[pageIndex].physicalAddress = physicalAddress;
    vmm.pageTable[pageIndex].present = 1;
    vmm.pageTable[pageIndex].writable = writable;
    vmm.pageTable[pageIndex].user = user;
    return 0;
}

void unmap_page(DWORD virtualAddress)
{
    DWORD pageIndex;
    pageIndex = virtualAddress / PAGE_SIZE;

    if (pageIndex < TOTAL_PAGES) 
    {
        vmm.pageTable[pageIndex].present = 0;
    }
}

DWORD translate_address(DWORD virtualAddress)
{
    DWORD pageIndex = virtualAddress / PAGE_SIZE;
    DWORD offset = virtualAddress % PAGE_SIZE;

    if
    (
        pageIndex >= TOTAL_PAGES ||
        !vmm.pageTable[pageIndex].present
    ) 
    {
        return 0;
    }

    return vmm.pageTable[pageIndex].physicalAddress + offset;
}

void* allocate_virt_mem
(
    DWORD virtualAddress,
    DWORD size,
    BYTE writable,
    BYTE user
)
{
    DWORD pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;

    for (DWORD i = 0; i < pages; i++) 
    {
        DWORD pageIndex = (virtualAddress / PAGE_SIZE) + i;
        
        if (pageIndex >= TOTAL_PAGES) 
        {
            return NULL;
        }

        DWORD physicalAddress = 
        (DWORD)&vmm.physicalMemory[pageIndex * PAGE_SIZE];
        
        map_page
        (
            virtualAddress + i * PAGE_SIZE,
            physicalAddress,
            writable,
            user
        );
    }

    return (void*)virtualAddress;
}

void free_virt_mem(DWORD virtualAddress, DWORD size)
{
    DWORD pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;

    for (DWORD i = 0; i < pages; i++) 
    {
        unmap_page(virtualAddress + i * PAGE_SIZE);
    }
}

int set_memory_protection
(
    DWORD virtualAddress,
    DWORD size,
    BYTE protectionFlags
)
{
    DWORD pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    DWORD pageIndex = virtualAddress / PAGE_SIZE;

    for (DWORD i = 0; i < pages; i++) 
    {
        DWORD currentPage = pageIndex + i;

        if (currentPage >= TOTAL_PAGES) 
        {
            return -1;
        }

        if (!vmm.pageTable[currentPage].present) 
        {
            return -1;
        }

        vmm.pageTable[currentPage].writable =
        (
            protectionFlags & PROT_WRITE
        ) ? 1 : 0;
        
        vmm.pageTable[currentPage].user =
        (
            protectionFlags & PROT_USER
        ) ? 1 : 0;
    }

    return 0;
}