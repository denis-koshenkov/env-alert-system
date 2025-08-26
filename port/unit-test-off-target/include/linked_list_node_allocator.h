#ifndef PORT_UNIT_TEST_OFF_TARGET_LINKED_LIST_NODE_ALLOCATOR_H
#define PORT_UNIT_TEST_OFF_TARGET_LINKED_LIST_NODE_ALLOCATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "utils/linked_list_private.h"

/**
 * @brief Allows to dynamically allocate memory for linked list nodes.
 *
 * CONFIG_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES defines the maximal nodes that can be allocated at the same time. If not
 * defined in config.h, defaults to 1.
 */

/**
 * @brief Allocate a linked list node.
 *
 * @return LinkedListNode* Pointer to the allocated memory for the linked list node if successful. If allocation fails,
 * returns NULL.
 */
LinkedListNode *linked_list_node_allocator_alloc();

/**
 * @brief Free a previously allocated linked list node.
 *
 * @param linked_list_node Node previously allocated and returned by @ref linked_list_node_allocator_alloc.
 *
 * @note Raises assert if @p block linked_list_node not point to a previously allocated node.
 * @note Raises assert if @p block linked_list_node to a previously allocated node that has already been freed.
 */
void linked_list_node_allocator_free(LinkedListNode *linked_list_node);

#ifdef __cplusplus
}
#endif

#endif
