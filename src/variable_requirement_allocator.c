#include "config.h"

#include "zephyr/sys/mem_blocks.h"

/**
 * @brief Increments x by the smallest possible amount to make it a multiple of 4.
 *
 * If x is already a multiple of 4, does nothing. Otherwise, it increases the value to make it a multiple of 4.
 * We add 3 to "push" x to the next multiple of 4, if it is not divisible by 4. Then, we clear the last 2 bits to align
 * down to the nearest multiple of 4.
 */
#define ROUND_UP_TO_4(x) (((x) + 3U) & ~0x3U)

/** The size of each block in the allocator should be a multiple of 4, because we are using 4-byte bounary alignment. If
 * the variable requirements size is not already a multiple of 4, we need to increase it to be a multiple of 4. For
 * example:
 *   - Variable size 8 -> block size 8 (already a multiple of 4)
 *   - Variable size 9 -> block size 12
 *   - Variable size 10 -> block size 12
 *   - Variable size 12 -> block size 12 (already a multiple of 4)
 * */
#define VARIABLE_REQUIREMENT_ALLOCATOR_BLOCK_SIZE                                                                      \
    ROUND_UP_TO_4(CONFIG_VARIABLE_REQUIREMENT_ALLOCATOR_VARIABLE_REQUIREMENT_SIZE)

SYS_MEM_BLOCKS_DEFINE_STATIC(allocator, VARIABLE_REQUIREMENT_ALLOCATOR_BLOCK_SIZE,
                             CONFIG_VARIABLE_REQUIREMENT_ALLOCATOR_MAX_NUM_VARIABLE_REQUIREMENTS, 4);

void *variable_requirement_allocator_alloc()
{
    void *block;
    int ret = sys_mem_blocks_alloc(allocator, 1, &block);
    EAS_ASSERT(ret != -EINVAL);
    return (ret == -ENOMEM) ? NULL : block;
}

void variable_requirement_allocator_free(void *buf)
{
    sys_mem_blocks_free(allocator, 1, &buf);
}
