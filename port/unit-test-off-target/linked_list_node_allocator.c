#include <stdint.h>

#include "linked_list_node_allocator.h"
#include "memory_block_allocator.h"
#include "config.h"

#ifndef CONFIG_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES
#define CONFIG_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES 1
#endif

static uint8_t nodes_buffer[sizeof(LinkedListNode) * CONFIG_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES];
static bool free_nodes_map[CONFIG_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES];

static MemoryBlockAllocator get_block_allocator_instance()
{
    static MemoryBlockAllocator instance;
    static bool is_created = false;
    if (!is_created) {
        instance = memory_block_allocator_create(CONFIG_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES, sizeof(LinkedListNode),
                                                 nodes_buffer, free_nodes_map);
        is_created = true;
    }
    return instance;
}

LinkedListNode *linked_list_node_allocator_alloc()
{
    return (LinkedListNode *)memory_block_allocator_alloc(get_block_allocator_instance());
}

void linked_list_node_allocator_free(LinkedListNode *linked_list_node)
{
    memory_block_allocator_free(get_block_allocator_instance(), (void *)linked_list_node);
}
