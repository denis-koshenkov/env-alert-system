#include <stddef.h>

#include "linked_list_node_allocator.h"

static LinkedListNode *prod_fw_linked_list_node_allocator_alloc()
{
    return NULL;
}

static void prod_fw_linked_list_node_allocator_free(LinkedListNode *linked_list_node)
{
}

LinkedListNode *(*linked_list_node_allocator_alloc)() = prod_fw_linked_list_node_allocator_alloc;
void (*linked_list_node_allocator_free)(LinkedListNode *linked_list_node) = prod_fw_linked_list_node_allocator_free;
