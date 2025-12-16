#include <stddef.h>

#include <zephyr/sys/mem_blocks.h>

#include "variable_requirement_allocator.h"
#include "config.h"
#include "eas_assert.h"
#include "utils/util.h"

/** Zephyr requirement: block size must be at least 4N bytes long, where N is greater than 0. */
#define VARIABLE_REQUIREMENT_ALLOCATOR_BLOCK_SIZE DIV4_UP(CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE)

/** The buffer must be aligned to an N-byte boundary, where N is a power of 2 larger than 2 (i.e. 4, 8, 16, ...). The
 * block size should also be a multiple of N. In our case, N is 4. We align the buffer to a 4-byte boundary and ensure
 * that the size of each block is a multiple of 4. 4 is chosen as the lowest allowed number, so that we do not have to
 * waste more space in each block. */
#define VARIABLE_REQUIREMENT_ALLOCATOR_BUF_ALIGNMENT 4

SYS_MEM_BLOCKS_DEFINE_STATIC(allocator, VARIABLE_REQUIREMENT_ALLOCATOR_BLOCK_SIZE,
                             CONFIG_VARIABLE_REQUIREMENT_ALLOCATOR_NUM_REQUIREMENTS,
                             VARIABLE_REQUIREMENT_ALLOCATOR_BUF_ALIGNMENT);

void *variable_requirement_allocator_alloc()
{
    void *out_block = NULL;
    int ret = sys_mem_blocks_alloc(&allocator, 1, &out_block);
    return (ret == 0) ? out_block : NULL;
}

void variable_requirement_allocator_free(void *buf)
{
    int ret = sys_mem_blocks_free(&allocator, 1, &buf);
    EAS_ASSERT(ret == 0);
}
