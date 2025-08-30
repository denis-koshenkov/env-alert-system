#include <stddef.h>

#include "utils/linked_list.h"
#include "utils/linked_list_private.h"
#include "linked_list_node_allocator.h"
#include "config.h"
#include "eas_assert.h"

#ifndef CONFIG_LINKED_LIST_MAX_NUM_INSTANCES
#define CONFIG_LINKED_LIST_MAX_NUM_INSTANCES 1
#endif

typedef struct LinkedListStruct {
    LinkedListNode *head;
} LinkedListStruct;

static struct LinkedListStruct instances[CONFIG_LINKED_LIST_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

LinkedList linked_list_create()
{
    EAS_ASSERT(instance_idx < CONFIG_LINKED_LIST_MAX_NUM_INSTANCES);
    struct LinkedListStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->head = NULL;
    return instance;
}

void linked_list_add(LinkedList self, void *element)
{
    EAS_ASSERT(self);

    LinkedListNode *new_node = linked_list_node_allocator_alloc();
    EAS_ASSERT(new_node);
    new_node->element = element;

    LinkedListNode *previous_head = self->head;
    self->head = new_node;
    new_node->next = previous_head;
}

void linked_list_for_each(LinkedList self, LinkedListForEachCb cb, void *user_data)
{
    EAS_ASSERT(self);
    EAS_ASSERT(cb);

    for (LinkedListNode *node = self->head; node != NULL; node = node->next) {
        cb(node->element, user_data);
    }
}

void linked_list_remove_on_condition(LinkedList self, LinkedListConditionCb cb, void *user_data)
{
    EAS_ASSERT(self);
    EAS_ASSERT(cb);

    LinkedListNode **prev_node_next = &(self->head);
    LinkedListNode *node = self->head;
    while (node != NULL) {
        if (cb(node->element, user_data)) {
            *prev_node_next = node->next;
            LinkedListNode *node_to_free = node;
            node = node->next;
            linked_list_node_allocator_free(node_to_free);
        } else {
            prev_node_next = &(node->next);
            node = node->next;
        }
    }
}
