#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "memory_block_allocator.h"

#define MEMORY_BLOCK_ALLOCATOR_TEST_MAX_NUM_BLOCKS 4
#define MEMORY_BLOCK_ALLOCATOR_TEST_MAX_BLOCK_SIZE 8
#define MEMORY_BLOCK_ALLOCATOR_TEST_BLOCKS_BUFFER_SIZE                                                                 \
    (MEMORY_BLOCK_ALLOCATOR_TEST_MAX_NUM_BLOCKS * MEMORY_BLOCK_ALLOCATOR_TEST_MAX_BLOCK_SIZE)

static uint8_t blocks[MEMORY_BLOCK_ALLOCATOR_TEST_BLOCKS_BUFFER_SIZE];
static bool free_blocks_map[MEMORY_BLOCK_ALLOCATOR_TEST_MAX_NUM_BLOCKS];

/**
 * @brief Validate block size and number of blocks being used in the test.
 *
 * Safety mechanism to make sure we do not exceeds the values defined by the macros. Otherwise, blocks and
 * free_blocks_map memory might be accessed out of bounds.
 *
 * @param block_size Block size.
 * @param num_blocks Number of blocks.
 *
 * @return true The values are valid.
 * @return false The values are invalid.
 */
static bool validate_block_size_num_blocks(size_t block_size, size_t num_blocks)
{
    return ((block_size <= MEMORY_BLOCK_ALLOCATOR_TEST_MAX_BLOCK_SIZE) &&
            (num_blocks <= MEMORY_BLOCK_ALLOCATOR_TEST_MAX_NUM_BLOCKS));
}

TEST_GROUP(MemoryBlockAllocator){};

TEST(MemoryBlockAllocator, CreateAssertsNumBlocks0)
{
    size_t block_size = 1;
    size_t num_blocks = 0;
    CHECK_TRUE(validate_block_size_num_blocks(block_size, num_blocks));
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("num_blocks > 0", "memory_block_allocator_create");
    memory_block_allocator_create(num_blocks, block_size, blocks, free_blocks_map);
}

TEST(MemoryBlockAllocator, CreateAssertsBlockSize0)
{
    size_t block_size = 0;
    size_t num_blocks = 1;
    CHECK_TRUE(validate_block_size_num_blocks(block_size, num_blocks));
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("block_size > 0", "memory_block_allocator_create");
    memory_block_allocator_create(num_blocks, block_size, blocks, free_blocks_map);
}

TEST(MemoryBlockAllocator, CreateAssertsBlocksIsNull)
{
    size_t block_size = 1;
    size_t num_blocks = 1;
    CHECK_TRUE(validate_block_size_num_blocks(block_size, num_blocks));
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("blocks", "memory_block_allocator_create");
    memory_block_allocator_create(num_blocks, block_size, NULL, free_blocks_map);
}

TEST(MemoryBlockAllocator, CreateAssertsFreeBlocksMapIsNull)
{
    size_t block_size = 1;
    size_t num_blocks = 1;
    CHECK_TRUE(validate_block_size_num_blocks(block_size, num_blocks));
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("free_blocks_map", "memory_block_allocator_create");
    memory_block_allocator_create(num_blocks, block_size, blocks, NULL);
}

TEST(MemoryBlockAllocator, AllocMoreThanNumBlocksReturnsNullOneBlock)
{
    size_t block_size = 1;
    size_t num_blocks = 1;
    CHECK_TRUE(validate_block_size_num_blocks(block_size, num_blocks));
    MemoryBlockAllocator memory_block_allocator =
        memory_block_allocator_create(num_blocks, block_size, blocks, free_blocks_map);
    void *allocated_block1 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_EQUAL(blocks, allocated_block1);
    void *allocated_block2 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block2 == NULL);
}

TEST(MemoryBlockAllocator, AllocMoreThanNumBlocksReturnsNullTwoBlocks)
{
    size_t block_size = 1;
    size_t num_blocks = 2;
    CHECK_TRUE(validate_block_size_num_blocks(block_size, num_blocks));
    MemoryBlockAllocator memory_block_allocator =
        memory_block_allocator_create(num_blocks, block_size, blocks, free_blocks_map);
    void *allocated_block1 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block1 != NULL);
    void *allocated_block2 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block2 != NULL);
    void *allocated_block3 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block3 == NULL);
}

TEST(MemoryBlockAllocator, FreeFreesBlockOneBlock)
{
    size_t block_size = 3;
    size_t num_blocks = 1;
    CHECK_TRUE(validate_block_size_num_blocks(block_size, num_blocks));
    MemoryBlockAllocator memory_block_allocator =
        memory_block_allocator_create(num_blocks, block_size, blocks, free_blocks_map);

    void *allocated_block1 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_EQUAL(blocks, allocated_block1);
    memory_block_allocator_free(memory_block_allocator, allocated_block1);
    void *allocated_block2 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_EQUAL(blocks, allocated_block2);
    void *allocated_block3 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block3 == NULL);
}

TEST(MemoryBlockAllocator, FreeFreesBlockTwoBlocks)
{
    size_t block_size = 4;
    size_t num_blocks = 2;
    CHECK_TRUE(validate_block_size_num_blocks(block_size, num_blocks));
    MemoryBlockAllocator memory_block_allocator =
        memory_block_allocator_create(num_blocks, block_size, blocks, free_blocks_map);

    void *allocated_block1 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block1 != NULL);
    void *allocated_block2 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block2 != NULL);
    memory_block_allocator_free(memory_block_allocator, allocated_block1);
    memory_block_allocator_free(memory_block_allocator, allocated_block2);

    void *allocated_block3 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block3 != NULL);
    void *allocated_block4 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block4 != NULL);
    void *allocated_block5 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block5 == NULL);
}

TEST(MemoryBlockAllocator, FreeRaisesAssertIfGivenInvalidBlock)
{
    size_t block_size = 3;
    size_t num_blocks = 2;
    CHECK_TRUE(validate_block_size_num_blocks(block_size, num_blocks));
    MemoryBlockAllocator memory_block_allocator =
        memory_block_allocator_create(num_blocks, block_size, blocks, free_blocks_map);

    /* We have 2 blocks of size 3. This will point to the second byte of the first block. This is not a valid block, so
     * we expect memory_block_allocator_free to raise an assert. */
    void *invalid_block = blocks + 1;
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("0", "get_block_id");
    memory_block_allocator_free(memory_block_allocator, invalid_block);
}

TEST(MemoryBlockAllocator, AllocRaisesAssertIfInstanceIsNull)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("self", "memory_block_allocator_alloc");
    memory_block_allocator_alloc(NULL);
}

TEST(MemoryBlockAllocator, FreeRaisesAssertIfInstanceIsNull)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("self", "memory_block_allocator_free");
    memory_block_allocator_free(NULL, blocks);
}

TEST(MemoryBlockAllocator, FreeRaisesAssertIfBlockIsNull)
{
    size_t block_size = 1;
    size_t num_blocks = 1;
    CHECK_TRUE(validate_block_size_num_blocks(block_size, num_blocks));
    MemoryBlockAllocator memory_block_allocator =
        memory_block_allocator_create(num_blocks, block_size, blocks, free_blocks_map);

    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("block", "memory_block_allocator_free");
    memory_block_allocator_free(memory_block_allocator, NULL);
}

TEST(MemoryBlockAllocator, FreeRaisesAssertIfBlockIsAlreadyFree)
{
    size_t block_size = 5;
    size_t num_blocks = 2;
    CHECK_TRUE(validate_block_size_num_blocks(block_size, num_blocks));
    MemoryBlockAllocator memory_block_allocator =
        memory_block_allocator_create(num_blocks, block_size, blocks, free_blocks_map);

    void *allocated_block = memory_block_allocator_alloc(memory_block_allocator);
    memory_block_allocator_free(memory_block_allocator, allocated_block);
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("!is_block_free(self, block_id)", "memory_block_allocator_free");
    memory_block_allocator_free(memory_block_allocator, allocated_block);
}

TEST(MemoryBlockAllocator, AllocAndFreeStressTest)
{
    size_t block_size = 8;
    size_t num_blocks = 4;
    CHECK_TRUE(validate_block_size_num_blocks(block_size, num_blocks));
    MemoryBlockAllocator memory_block_allocator =
        memory_block_allocator_create(num_blocks, block_size, blocks, free_blocks_map);

    void *allocated_block1 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block1 != NULL);
    void *allocated_block2 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block2 != NULL);
    void *allocated_block3 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block3 != NULL);
    void *allocated_block4 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block4 != NULL);
    /* We have 4 blocks, so this alloc should fail */
    void *allocated_block5 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block5 == NULL);

    /* Free one block */
    memory_block_allocator_free(memory_block_allocator, allocated_block2);

    /* Since we freed one block, there should be only one available */
    void *allocated_block6 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block6 != NULL);
    void *allocated_block7 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block7 == NULL);

    /* Free all but one block - allocated_block3 is the only one still allocated*/
    memory_block_allocator_free(memory_block_allocator, allocated_block4);
    memory_block_allocator_free(memory_block_allocator, allocated_block1);
    memory_block_allocator_free(memory_block_allocator, allocated_block6);

    void *allocated_block8 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block8 != NULL);
    void *allocated_block9 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block9 != NULL);
    void *allocated_block10 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block10 != NULL);
    /* We have 4 blocks allocated, so this alloc should fail */
    void *allocated_block11 = memory_block_allocator_alloc(memory_block_allocator);
    CHECK_TRUE(allocated_block11 == NULL);

    /* Valid blocks now: 3, 8, 9, 10 */
    memory_block_allocator_free(memory_block_allocator, allocated_block10);
    memory_block_allocator_free(memory_block_allocator, allocated_block8);
    memory_block_allocator_free(memory_block_allocator, allocated_block3);
    memory_block_allocator_free(memory_block_allocator, allocated_block9);
}
