#include "memory_block_allocator.h"
#include "eas_assert.h"

#ifndef CONFIG_MEMORY_BLOCK_ALLOCATOR_MAX_NUM_INSTANCES
#define CONFIG_MEMORY_BLOCK_ALLOCATOR_MAX_NUM_INSTANCES 1
#endif

struct MemoryBlockAllocatorStruct {
    void *blocks;
    bool *free_blocks_map;
};

static struct MemoryBlockAllocatorStruct instances[CONFIG_MEMORY_BLOCK_ALLOCATOR_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

MemoryBlockAllocator memory_block_allocator_create(size_t num_blocks, size_t block_size, void *blocks,
                                                   bool *free_blocks_map)
{
    EAS_ASSERT(num_blocks > 0);
    EAS_ASSERT(block_size > 0);
    EAS_ASSERT(blocks);
    EAS_ASSERT(free_blocks_map);

    EAS_ASSERT(instance_idx < CONFIG_MEMORY_BLOCK_ALLOCATOR_MAX_NUM_INSTANCES);
    struct MemoryBlockAllocatorStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->blocks = blocks;
    instance->free_blocks_map = free_blocks_map;
    instance->free_blocks_map[0] = true;

    return instance;
}

void *memory_block_allocator_alloc(MemoryBlockAllocator self)
{
    if (self->free_blocks_map[0]) {
        self->free_blocks_map[0] = false;
        return self->blocks;
    } else {
        return NULL;
    }
}
