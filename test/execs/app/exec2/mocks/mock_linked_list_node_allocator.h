#ifndef MOCK_LINKED_LIST_NODE_ALLOCATOR_H
#define MOCK_LINKED_LIST_NODE_ALLOCATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "utils/linked_list_private.h"

LinkedListNode *linked_list_node_allocator_alloc();

void linked_list_node_allocator_free(LinkedListNode *linked_list_node);

#ifdef __cplusplus
}
#endif

#endif
