#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "memory_block_allocator.h"

#define MEMORY_BLOCK_ALLOCATOR_TEST_MAX_NUM_BLOCKS 1
#define MEMORY_BLOCK_ALLOCATOR_TEST_MAX_BLOCK_SIZE 1
#define MEMORY_BLOCK_ALLOCATOR_TEST_BLOCKS_BUFFER_SIZE                                                                 \
    (MEMORY_BLOCK_ALLOCATOR_TEST_MAX_NUM_BLOCKS * MEMORY_BLOCK_ALLOCATOR_TEST_MAX_BLOCK_SIZE)

/* Buffers to pass to create() - these are used by the memory_block_allocator CUT */
static uint8_t blocks[MEMORY_BLOCK_ALLOCATOR_TEST_BLOCKS_BUFFER_SIZE];
static bool free_blocks_map[MEMORY_BLOCK_ALLOCATOR_TEST_MAX_NUM_BLOCKS];

/* Buffers for validating test results */
static void *allocated_blocks[MEMORY_BLOCK_ALLOCATOR_TEST_MAX_NUM_BLOCKS];

static bool is_valid_block(void *block, size_t block_size, size_t total_num_blocks)
{
    uint8_t *current_block = blocks;
    while (current_block < blocks + (block_size * total_num_blocks)) {
        if (block == (void *)current_block) {
            /* Found the block*/
            return true;
        } else {
            current_block += block_size;
        }
    }
    return false;
}

static void validate_allocated_blocks(size_t block_size, size_t num_blocks_to_validate, size_t total_num_blocks)
{
    /* Check that all allocated blocks are valid blocks */
    for (size_t i = 0; i < MEMORY_BLOCK_ALLOCATOR_TEST_MAX_NUM_BLOCKS; i++) {
        CHECK_TRUE(is_valid_block(allocated_blocks[i]));
    }

    /* TODO: Check that there are no duplicates among allocated blocks */
}

TEST_GROUP(MemoryBlockAllocator){};

TEST(MemoryBlockAllocator, CreateAssertsNumBlocks0)
{
    size_t block_size = 1;
    size_t num_blocks = 0;
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("num_blocks > 0", "memory_block_allocator_create");
    memory_block_allocator_create(num_blocks, block_size, blocks, free_blocks_map);
}

TEST(MemoryBlockAllocator, CreateAssertsBlockSize0)
{
    size_t block_size = 0;
    size_t num_blocks = 1;
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("block_size > 0", "memory_block_allocator_create");
    memory_block_allocator_create(num_blocks, block_size, blocks, free_blocks_map);
}

TEST(MemoryBlockAllocator, CreateAssertsBlocksIsNull)
{
    size_t block_size = 1;
    size_t num_blocks = 1;
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("blocks", "memory_block_allocator_create");
    memory_block_allocator_create(num_blocks, block_size, NULL, free_blocks_map);
}

TEST(MemoryBlockAllocator, CreateAssertsFreeBlocksMapIsNull)
{
    size_t block_size = 1;
    size_t num_blocks = 1;
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("free_blocks_map", "memory_block_allocator_create");
    memory_block_allocator_create(num_blocks, block_size, blocks, NULL);
}

TEST(MemoryBlockAllocator, AllocMoreThanNumBlocksReturnsNullOneBlock)
{
    size_t block_size = 1;
    size_t num_blocks = 1;
    MemoryBlockAllocator memory_block_allocator =
        memory_block_allocator_create(num_blocks, block_size, blocks, free_blocks_map);
    void *allocated_block1 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_EQUAL(blocks, allocated_block1);
    void *allocated_block2 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_EQUAL(NULL, allocated_block2);
}

TEST(MemoryBlockAllocator, AllocMoreThanNumBlocksReturnsNullTwoBlocks)
{
    size_t block_size = 1;
    size_t num_blocks = 2;
    MemoryBlockAllocator memory_block_allocator =
        memory_block_allocator_create(num_blocks, block_size, blocks, free_blocks_map);
    void *allocated_block0 = memory_block_allocator_alloc(memory_block_allocator);
    allocated_blocks[0] = allocated_block0;
    void *allocated_block1 = memory_block_allocator_alloc(memory_block_allocator);
    allocated_blocks[1] = allocated_block1;
    validate_allocated_blocks();
}
