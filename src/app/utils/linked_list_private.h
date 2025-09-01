#ifndef ENV_ALERT_SYSTEM_SRC_APP_UTILS_LINKED_LIST_PRIVATE_H
#define ENV_ALERT_SYSTEM_SRC_APP_UTILS_LINKED_LIST_PRIVATE_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Defined in a separate header so that linked_list_node_allocator module can include this header and know the size of
 * the linked list node that it needs to allocate at compile time. */
typedef struct LinkedListNode {
    void *element;
    struct LinkedListNode *next;
} LinkedListNode;

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_UTILS_LINKED_LIST_PRIVATE_H */
