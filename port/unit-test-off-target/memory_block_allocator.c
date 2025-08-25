#include "memory_block_allocator.h"
#include "eas_assert.h"

MemoryBlockAllocator memory_block_allocator_create(size_t num_blocks, size_t block_size, uint8_t *blocks,
                                                   bool *free_blocks_map)
{
    EAS_ASSERT(num_blocks > 0);
    return NULL;
}
