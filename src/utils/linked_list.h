#ifndef UTILS_LINKED_LIST_H
#define UTILS_LINKED_LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

typedef struct LinkedListStruct *LinkedList;

typedef void (*LinkedListForEachCb)(void *element, void *user_data);
typedef bool (*LinkedListConditionCb)(void *element);

LinkedList linked_list_create();

void linked_list_add(LinkedList self, void *element);

void linked_list_for_each(LinkedList self, LinkedListForEachCb cb, void *user_data);

void linked_list_remove_on_condition(LinkedList self, LinkedListConditionCb cb);

#ifdef __cplusplus
}
#endif

#endif
