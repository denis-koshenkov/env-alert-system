#include <stdint.h>

#include "memory_block_allocator.h"
#include "eas_assert.h"
#include "config.h"

#ifndef CONFIG_MEMORY_BLOCK_ALLOCATOR_MAX_NUM_INSTANCES
#define CONFIG_MEMORY_BLOCK_ALLOCATOR_MAX_NUM_INSTANCES 1
#endif

struct MemoryBlockAllocatorStruct {
    size_t num_blocks;
    size_t block_size;
    uint8_t *blocks;
    bool *free_blocks_map;
};

static struct MemoryBlockAllocatorStruct instances[CONFIG_MEMORY_BLOCK_ALLOCATOR_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

static bool is_block_free(MemoryBlockAllocator self, size_t block_id)
{
    EAS_ASSERT(block_id < self->num_blocks);
    return self->free_blocks_map[block_id];
}

static void *get_block_memory(MemoryBlockAllocator self, size_t block_id)
{
    EAS_ASSERT(block_id < self->num_blocks);
    return self->blocks + (block_id * self->block_size);
}

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

    instance->num_blocks = num_blocks;
    instance->block_size = block_size;
    instance->blocks = (uint8_t *)blocks;
    instance->free_blocks_map = free_blocks_map;
    /* All blocks are initially free */
    for (size_t i = 0; i < num_blocks; i++) {
        instance->free_blocks_map[i] = true;
    }

    return instance;
}

void *memory_block_allocator_alloc(MemoryBlockAllocator self)
{
    for (size_t block_id = 0; block_id < self->num_blocks; block_id++) {
        if (is_block_free(self, block_id)) {
            self->free_blocks_map[block_id] = false;
            return get_block_memory(self, block_id);
        }
    }
    /* There are no free blocks */
    return NULL;
}

void memory_block_allocator_free(MemoryBlockAllocator self, void *block)
{
    self->free_blocks_map[0] = true;
}
