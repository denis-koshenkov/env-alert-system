#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_LINKED_LIST_NODE_ALLOCATOR_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_LINKED_LIST_NODE_ALLOCATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "utils/linked_list_private.h"

/** Defined as function pointers here, so that we can use two versions of linked list node allocator in our tests:
 *    1. Mock linked list node allocator. Used when the allocator calls are relevant for the test - when the test
 * verifies when and how the module interacts with the allocator. For example, the tests for the linked list module need
 * to verify that linked list function allocate and free nodes as expected.
 *    2. Fake linked list node allocator. A simple allocator implementation that uses our memory block allocator. Useful
 * when calls to linked list node allocator are not relevant for the test. We just want the allocator to work so that
 * the test can focus on testing other things. For example, LedManager tests. LedManager uses a linked list, which uses
 * the linked list allocator. However, LedManager tests do not care when and how the linked list allocator is called -
 * these tests verify the behavior of the LedManager. It is irrelevant for the test how exactly the LedManager stores
 * its state, so the test should not need to expect mocked calls to the linked list allocator.
 *
 * By default, these function pointers are set to the mock linked list node allocator functions. The tests can directly
 * set these function pointers to other implementations, such as the fake linked list node allocator implementation.
 */

extern LinkedListNode *(*linked_list_node_allocator_alloc)();

extern void (*linked_list_node_allocator_free)(LinkedListNode *linked_list_node);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_LINKED_LIST_NODE_ALLOCATOR_H */
