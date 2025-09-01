#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_LINKED_LIST_NODE_ALLOCATOR_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_LINKED_LIST_NODE_ALLOCATOR_H

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

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_LINKED_LIST_NODE_ALLOCATOR_H */
