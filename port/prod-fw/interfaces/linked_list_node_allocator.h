#ifndef PORT_PROD_FW_INTERFACES_LINKED_LIST_NODE_ALLOCATOR_H
#define PORT_PROD_FW_INTERFACES_LINKED_LIST_NODE_ALLOCATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "utils/linked_list_private.h"

/**
 * @brief Abstract interface for the application to allocate memory for linked list nodes.
 *
 * This module will implement zephyr's memory block allocator under the hood. Right now, it is defined as empty only to
 * avoid include and linker errors during the build.
 */

LinkedListNode *linked_list_node_allocator_alloc();

void linked_list_node_allocator_free(LinkedListNode *linked_list_node);

#ifdef __cplusplus
}
#endif

#endif
