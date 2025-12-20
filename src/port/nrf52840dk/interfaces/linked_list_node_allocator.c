#include <stddef.h>

#include <zephyr/sys/mem_blocks.h>

#include "linked_list_node_allocator.h"
#include "util.h"
#include "config.h"
#include "eas_assert.h"

/** Zephyr requirement: block size must be at least 4N bytes long, where N is greater than 0. */
#define LINKED_LIST_NODE_ALLOCATOR_BLOCK_SIZE DIV4_UP(sizeof(LinkedListNode))

/** The buffer must be aligned to an N-byte boundary, where N is a power of 2 larger than 2 (i.e. 4, 8, 16, ...). The
 * block size should also be a multiple of N. In our case, N is 4. We align the buffer to a 4-byte boundary and ensure
 * that the size of each block is a multiple of 4. 4 is chosen as the lowest allowed number, so that we do not have to
 * waste more space in each block. */
#define LINKED_LIST_NODE_ALLOCATOR_BUF_ALIGNMENT 4

SYS_MEM_BLOCKS_DEFINE_STATIC(linked_list_node_allocator, LINKED_LIST_NODE_ALLOCATOR_BLOCK_SIZE,
                             CONFIG_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES, LINKED_LIST_NODE_ALLOCATOR_BUF_ALIGNMENT);

static LinkedListNode *zephyr_linked_list_node_allocator_alloc()
{
    void *out_block = NULL;
    int ret = sys_mem_blocks_alloc(&linked_list_node_allocator, 1, &out_block);
    return (ret == 0) ? ((LinkedListNode *)out_block) : NULL;
}

static void zephyr_linked_list_node_allocator_free(LinkedListNode *linked_list_node)
{
    int ret = sys_mem_blocks_free(&linked_list_node_allocator, 1, (void **)&linked_list_node);
    EAS_ASSERT(ret == 0);
}

LinkedListNode *(*linked_list_node_allocator_alloc)() = zephyr_linked_list_node_allocator_alloc;
void (*linked_list_node_allocator_free)(LinkedListNode *linked_list_node) = zephyr_linked_list_node_allocator_free;
