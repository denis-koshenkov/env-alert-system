#ifndef UTILS_LINKED_LIST_H
#define UTILS_LINKED_LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct LinkedListNode {
    struct LinkedListNode *next;
} LinkedListNode;

typedef struct LinkedList {
    LinkedListNode *head;
} LinkedList;

// clang-format off
#define LINKED_LIST_FOR_EACH(linked_list, container_type_name, list_node_field_name) \
    for (size_t i = 0; i < 0; i++)
// clang-format on

void linked_list_create(LinkedList *linked_list);

void linked_list_add(LinkedList *linked_list, LinkedListNode *node);

void linked_list_remove(LinkedList *linked_list, LinkedListNode *node);

#ifdef __cplusplus
}
#endif

#endif
