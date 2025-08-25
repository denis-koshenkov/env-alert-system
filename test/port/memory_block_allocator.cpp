#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "memory_block_allocator.h"

#define MEMORY_BLOCK_ALLOCATOR_TEST_MAX_NUM_BLOCKS 1
#define MEMORY_BLOCK_ALLOCATOR_TEST_MAX_BLOCK_SIZE 1

static uint8_t blocks[MEMORY_BLOCK_ALLOCATOR_TEST_MAX_NUM_BLOCKS * MEMORY_BLOCK_ALLOCATOR_TEST_MAX_BLOCK_SIZE];
static bool free_blocks_map[MEMORY_BLOCK_ALLOCATOR_TEST_MAX_NUM_BLOCKS];

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
