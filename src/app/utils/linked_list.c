#include <stddef.h>
#include <stdint.h>

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

/**
 * @brief Condition callback that returns true if the element passed as user data matches the current element.
 *
 * This function is passed as the condition callback to @ref linked_list_remove_on_condition by @ref linked_list_remove.
 *
 * @param element Current element in the iteration.
 * @param user_data Element that should be removed.
 *
 * @return true @p element is the element that should be removed.
 * @return false @p element is not the element that should be removed.
 */
static bool remove_element_condition_cb(void *element, void *user_data)
{
    void *element_to_remove = user_data;
    return (element == element_to_remove);
}

LinkedList linked_list_create()
{
    EAS_ASSERT(instance_idx < CONFIG_LINKED_LIST_MAX_NUM_INSTANCES);
    struct LinkedListStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->head = NULL;
    return instance;
}

void linked_list_prepend(LinkedList self, void *element)
{
    EAS_ASSERT(self);

    LinkedListNode *new_node = linked_list_node_allocator_alloc();
    EAS_ASSERT(new_node);
    new_node->element = element;

    LinkedListNode *previous_head = self->head;
    self->head = new_node;
    new_node->next = previous_head;
}

void linked_list_append(LinkedList self, void *element)
{
    EAS_ASSERT(self);

    LinkedListNode *new_node = linked_list_node_allocator_alloc();
    EAS_ASSERT(new_node);
    new_node->element = element;
    new_node->next = NULL;

    if (self->head == NULL) {
        self->head = new_node;
    } else {
        LinkedListNode *current_node = self->head;
        while (current_node->next != NULL) {
            current_node = current_node->next;
        }
        current_node->next = new_node;
    }
}

bool linked_list_remove(LinkedList self, void *element)
{
    EAS_ASSERT(self);

    size_t num_removed =
        linked_list_remove_on_condition_with_limit(self, 1, remove_element_condition_cb, element, NULL, NULL);
    /* At most one element was supposed to be removed from the list */
    EAS_ASSERT(num_removed <= 1);
    /* 1 -> element was removed, 0 -> element was not removed */
    return (num_removed == 1);
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

    /* Does not matter how many elements are removed - do not care about return value */
    linked_list_remove_on_condition_with_limit(self, LINKED_LIST_REMOVE_NO_LIMIT, cb, user_data, NULL, NULL);
}

size_t linked_list_remove_on_condition_with_limit(LinkedList self, size_t max_num_elements_to_remove,
                                                  LinkedListConditionCb condition_cb, void *condition_cb_user_data,
                                                  LinkedListPreRemoveCb pre_remove_cb, void *pre_remove_cb_user_data)
{
    EAS_ASSERT(self);
    EAS_ASSERT(condition_cb);

    size_t num_removed_elements = 0;
    LinkedListNode **prev_node_next = &(self->head);
    LinkedListNode *node = self->head;
    while (node != NULL) {
        if (num_removed_elements >= max_num_elements_to_remove) {
            break;
        }
        if (condition_cb(node->element, condition_cb_user_data)) {
            if (pre_remove_cb) {
                pre_remove_cb(node->element, pre_remove_cb_user_data);
            }
            *prev_node_next = node->next;
            LinkedListNode *node_to_free = node;
            node = node->next;
            linked_list_node_allocator_free(node_to_free);
            num_removed_elements++;
        } else {
            prev_node_next = &(node->next);
            node = node->next;
        }
    }
    return num_removed_elements;
}

void *linked_list_iterator_init(LinkedList self)
{
    EAS_ASSERT(self);

    return self->head;
}

bool linked_list_iterator_next(void **iterator, void **element)
{
    EAS_ASSERT(iterator);
    EAS_ASSERT(element);

    LinkedListNode *current_node = (LinkedListNode *)*iterator;
    if (current_node) {
        *element = current_node->element;
        *iterator = current_node->next;
        return true;
    } else {
        return false;
    }
}
