#ifndef UTILS_LINKED_LIST_H
#define UTILS_LINKED_LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct LinkedListStruct *LinkedList;

typedef void (*for_each_cb)(void *element, void *user_data);

LinkedList linked_list_create();

void linked_list_add(LinkedList self, void *element);

void linked_list_for_each(LinkedList self, for_each_cb cb, void *user_data);

#ifdef __cplusplus
}
#endif

#endif
