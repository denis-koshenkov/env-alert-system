#ifndef PORT_UNIT_TEST_OFF_TARGET_MEMORY_BLOCK_ALLOCATOR_H
#define PORT_UNIT_TEST_OFF_TARGET_MEMORY_BLOCK_ALLOCATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdbool.h>

typedef struct MemoryBlockAllocatorStruct *MemoryBlockAllocator;

MemoryBlockAllocator memory_block_allocator_create(size_t num_blocks, size_t block_size, void *blocks,
                                                   bool *free_blocks_map);

void *memory_block_allocator_alloc(MemoryBlockAllocator self);

void memory_block_allocator_free(MemoryBlockAllocator self, void *block);

#ifdef __cplusplus
}
#endif

#endif
