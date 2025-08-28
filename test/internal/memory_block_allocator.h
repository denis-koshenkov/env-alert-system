#ifndef TEST_INTERNAL_MEMORY_BLOCK_ALLOCATOR_H
#define TEST_INTERNAL_MEMORY_BLOCK_ALLOCATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Allows to dynamically allocate blocks of fixed size from a fixed pool of memory.
 */
typedef struct MemoryBlockAllocatorStruct *MemoryBlockAllocator;

/**
 * @brief Create a memory block allocator instance.
 *
 * @param num_blocks Number of blocks.
 * @param block_size The size of each block in bytes.
 * @param blocks The caller should provide memory of size @p num_blocks * @p block_size bytes. This is the memory pool
 * from which the blocks will be allocated.
 * @param free_blocks_map The caller should provide a pointer to memory that fits @p num_blocks elements of type bool,
 * i.e. a bool array of size @p num_blocks. This memory is used to keep track of which blocks are free and which are
 * allocated.
 *
 * @return MemoryBlockAllocator Created memory block allocator instance.
 */
MemoryBlockAllocator memory_block_allocator_create(size_t num_blocks, size_t block_size, void *blocks,
                                                   bool *free_blocks_map);

/**
 * @brief Allocate a block.
 *
 * @param self MemoryBlockAllocator instance returned by @ref memory_block_allocator_create.
 * @return * void* Pointer to the allocated block if allocation was successful (there was a free block), NULL otherwise.
 */
void *memory_block_allocator_alloc(MemoryBlockAllocator self);

/**
 * @brief Free a previously allocated block.
 *
 * @param self MemoryBlockAllocator instance returned by @ref memory_block_allocator_create.
 * @param block Block previously allocated and returned by @ref memory_block_allocator_alloc.
 *
 * @note Raises assert if @p block does not point to a valid block.
 * @note Raises assert if @p block points to a valid block that is already free.
 */
void memory_block_allocator_free(MemoryBlockAllocator self, void *block);

#ifdef __cplusplus
}
#endif

#endif
