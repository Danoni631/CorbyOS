#ifndef _ALLOC_H
#define _ALLOC_H

#include "include/stdint.h"

#define MEMORY_POOL 0xF00000
#define BLOCK_SIZE sizeof(MemoryBlock)

typedef struct MemoryBlock
{
    struct MemoryBlock* next;
    DWORD size;
    BYTE used;
} MemoryBlock;

typedef struct
{
    MemoryBlock* freeBlocks;
    BYTE memory[MEMORY_POOL];
} MemoryManager;

void init_memory();
void* alloc_memory(DWORD size);
void free_memory(void* ptr);
void* callocate_memory(int num, INT size);
void* realloc_memory(void* ptr, DWORD newSize);

#endif // _ALLOC_H