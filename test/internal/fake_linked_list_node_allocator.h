#ifndef TEST_INTERNAL_FAKE_LINKED_LIST_NODE_ALLOCATOR_H
#define TEST_INTERNAL_FAKE_LINKED_LIST_NODE_ALLOCATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "utils/linked_list_private.h"

/**
 * @brief Allows to dynamically allocate memory for linked list nodes during tests.
 *
 * CONFIG_FAKE_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES defines the maximal nodes that can be allocated at the same time. If
 * not defined in config.h, defaults to 1.
 *
 * Under the hood, uses the memory_block_allocator module. It is very much a real node allocator - it is only called
 * fake because it is called from the test program and not by production code directly.
 */

/**
 * @brief Allocate a linked list node.
 *
 * @return LinkedListNode* Pointer to the allocated memory for the linked list node if successful. If allocation fails,
 * returns NULL.
 */
LinkedListNode *fake_linked_list_node_allocator_alloc();

/**
 * @brief Free a previously allocated linked list node.
 *
 * @param linked_list_node Node previously allocated and returned by @ref linked_list_node_allocator_alloc.
 *
 * @note Raises assert if @p block linked_list_node not point to a previously allocated node.
 * @note Raises assert if @p block linked_list_node to a previously allocated node that has already been freed.
 */
void fake_linked_list_node_allocator_free(LinkedListNode *linked_list_node);

#ifdef __cplusplus
}
#endif

#endif
