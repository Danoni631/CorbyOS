/*
    CODED BY Danoni631, on 07/14/2026
    Is a memory allocation system thats uses memory blocks.
*/

#include "mem.h"
#include "alloc.h"

MemoryManager manager;

void init_memory()
{
    manager.freeBlocks = (MemoryBlock*)manager.memory;
    manager.freeBlocks->next = NULL;
    manager.freeBlocks->size = MEMORY_POOL - BLOCK_SIZE;
    manager.freeBlocks->used = 0;
}

void* alloc_memory(DWORD size)
{
    MemoryBlock* block = manager.freeBlocks;
    MemoryBlock* prev = NULL;

    size = (size + BLOCK_SIZE - 1) / BLOCK_SIZE * BLOCK_SIZE;

    while (block) 
    {
        if (!block->used && block->size >= size) 
        {
            if (block->size > size + BLOCK_SIZE) 
            {
                MemoryBlock* newBlock =
                (MemoryBlock*)((BYTE*)block + BLOCK_SIZE + size);
                newBlock->size = block->size - size - BLOCK_SIZE;
                newBlock->used = 0;
                newBlock->next = block->next;
                block->next = newBlock;
                block->size = size;
            }

            block->used = 1;
            return (BYTE*)block + BLOCK_SIZE;
        }

        prev = block;
        block = block->next;
    }

    return NULL;
}

void free_memory(void* ptr)
{
    if (!ptr) return;

    MemoryBlock* block = (MemoryBlock*)((BYTE*)ptr - BLOCK_SIZE);
    block->used = 0;

    MemoryBlock* current = manager.freeBlocks;
    
    while (current) 
    {
        if
        (
            (BYTE*)current +
            BLOCK_SIZE +
            current->size ==
            (BYTE*)block
        ) 
        {
            current->size += BLOCK_SIZE + block->size;
            current->next = block->next;
            
            return;
        }

        if
        (
            (BYTE*)block +
            BLOCK_SIZE +
            block->size == (BYTE*)current
        )
        {
            block->size += BLOCK_SIZE + current->size;
            block->next = current->next;

            if (manager.freeBlocks == current) 
            {
                manager.freeBlocks = block;
            }
            
            return;
        }

        current = current->next;
    }

    block->next = manager.freeBlocks;
    manager.freeBlocks = block;
}

void* callocate_memory(int num, INT size)
{
    int total = num * size;

    void* ptr = alloc_memory(total);
    
    if (!ptr)
    {
        return NULL;
    }

    memset(ptr, 0x00, total);
    
    return ptr;
}

void* realloc_memory(void* ptr, DWORD newSize)
{
    if(ptr == NULL)
    {
        return alloc_memory(newSize);
    }

    if(newSize == 0)
    {
        free_memory(ptr);
        return NULL;
    }

    MemoryBlock* block = (MemoryBlock*)((BYTE*)ptr - BLOCK_SIZE);
    #define ALIGN 4
    newSize = (newSize + ALIGN - 1) & ~(ALIGN - 1);

    if(block->size >= newSize)
    {
        return ptr;
    }

    MemoryBlock* next = block->next;

    if
    (
        next && !next->used &&
        (
            block->size + BLOCK_SIZE + next->size
        ) >= newSize
    )
    {
        block->size += BLOCK_SIZE + next->size;
        block->next = next->next;
        return ptr;
    }

    void* newPtr = alloc_memory(newSize);

    if(newPtr == NULL)
    {
        return NULL;
    }

    memcpy(newPtr, ptr, block->size);
    free_memory(ptr);
    return newPtr;
}