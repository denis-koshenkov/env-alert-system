#include <stddef.h>

#include "utils/linked_list.h"
#include "config.h"
#include "eas_assert.h"

#ifndef CONFIG_LINKED_LIST_MAX_NUM_INSTANCES
#define CONFIG_LINKED_LIST_MAX_NUM_INSTANCES 1
#endif

typedef struct LinkedListNode {
} LinkedListNode;

typedef struct LinkedListStruct {
    void *element;
} LinkedListStruct;

static struct LinkedListStruct instances[CONFIG_LINKED_LIST_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

LinkedList linked_list_create()
{
    EAS_ASSERT(instance_idx < CONFIG_LINKED_LIST_MAX_NUM_INSTANCES);
    struct LinkedListStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->element = NULL;
    return instance;
}

void linked_list_add(LinkedList self, void *element)
{
    self->element = element;
}

void linked_list_for_each(LinkedList self, for_each_cb cb, void *user_data)
{
    if (self->element) {
        cb(self->element, NULL);
    }
}
