#ifndef ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_LINKED_LIST_NODE_ALLOCATOR_H
#define ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_LINKED_LIST_NODE_ALLOCATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "utils/linked_list_private.h"

/**
 * @brief Block memory allocator for linked list nodes.
 *
 * The implementation of this interface must be able to simulatensously allocate
 * CONFIG_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES memory blocks of size sizeof(LinkedListNode) bytes. Each memory
 * block is one linked list node.
 *
 * This interface is implemented as function pointers so that the interface functions can be changed during runtime.
 * This is necessary for the unit test port, which uses different mocks/stubs for different tests.
 *
 * The implementation should implement the interface in a .c file, and then assign the function pointers below to the
 * function implementations.
 */

/**
 * @brief Allocate memory for a linked list node.
 *
 * @return LinkedListNode* If successful, points to allocated memory for linked list node. If failed due to being out of
 * memory, returns NULL.
 */
extern LinkedListNode *(*linked_list_node_allocator_alloc)();

/**
 * @brief Free a previously allocated linked list node.
 *
 * @param linked_list_node Linked list node buffer previously returned by @ref linked_list_node_allocator_alloc.
 */
extern void (*linked_list_node_allocator_free)(LinkedListNode *linked_list_node);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_LINKED_LIST_NODE_ALLOCATOR_H */
