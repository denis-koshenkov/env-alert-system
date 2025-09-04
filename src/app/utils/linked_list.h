#ifndef ENV_ALERT_SYSTEM_SRC_APP_UTILS_LINKED_LIST_H
#define ENV_ALERT_SYSTEM_SRC_APP_UTILS_LINKED_LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

/**
 * @brief Singly linked list.
 */
typedef struct LinkedListStruct *LinkedList;

/**
 * @brief Defines callback type to execute for each element as a part of @ref linked_list_for_each.
 *
 * @ref linked_list_for_each executes this callback for each element present in the list.
 *
 * @param element Element for which the callback is being executed.
 * @param user_data User data that is passed as a parameter to @ref linked_list_for_each. This is how the caller of @ref
 * linked_list_for_each can pass data to the callback implementation.
 */
typedef void (*LinkedListForEachCb)(void *element, void *user_data);

/**
 * @brief Defines callback type to execute for each element as a part of @ref linked_list_remove_on_condition.
 *
 * This callback is executed for each element. Based on the return value of the callback implementation, the
 * implementation of @ref linked_list_remove_on_condition either keeps the element in the list, or removes it.
 *
 * @param element Element is passed to the callback as a parameter. This is useful when the condition depends on some
 * data that is stored as a part of the element.
 * @param user_data User data that is passed as a parameter to @ref linked_list_remove_on_condition. This is how the
 * caller of @ref linked_list_remove_on_condition can pass data to the callback implementation.
 *
 * @return true The element will be removed from the list.
 * @return false The element will be kept in the list.
 */
typedef bool (*LinkedListConditionCb)(void *element, void *user_data);

/**
 * @brief Create a linked list instance.
 *
 * @return LinkedList Created linked list instance.
 */
LinkedList linked_list_create();

/**
 * @brief Add an element to the beginning of the linked list.
 *
 * More efficient than @ref linked_list_append, because there is no need to iterate till the end of the list.
 *
 * Since the element is added to the beginning of the list, the subsequent calls to @ref linked_list_for_each or @ref
 * linked_list_remove_on_condition will handle this newly added element first, before all the elements added previously.
 *
 * @param self Linked list instance returned by @ref linked_list_create.
 * @param element Element to add to the list.
 */
void linked_list_prepend(LinkedList self, void *element);

/**
 * @brief Remove an element from the list.
 *
 * @param self Linked list instance returned by @ref linked_list_create.
 * @param element Element to remove.
 *
 * @return true The element was removed from the list.
 * @return false The element is not present in the list, so it was not removed.
 *
 * @note If multiple occurrences of the elements are present in the list, only the first one is removed.
 */
bool linked_list_remove(LinkedList self, void *element);

/**
 * @brief Execute a callback for each element in the list.
 *
 * @param self Linked list instance returned by @ref linked_list_create.
 * @param cb Callback to execute.
 * @param user_data User data to pass to the callback. Can be NULL.
 *
 * @note Fires an assert if @p cb is NULL. Calling this function with @p cb equal to NULL would be equivalent to not
 * calling this function at all.
 */
void linked_list_for_each(LinkedList self, LinkedListForEachCb cb, void *user_data);

/**
 * @brief Remove all elements from the list that satisfy a certain condition.
 *
 * @param self Linked list instance returned by @ref linked_list_create.
 * @param cb This callback is called for each element in the list. If the callback for the element returns true, then
 * the element is removed from the list. If the callback for the element returns false, the element is kept in the list.
 * @param user_data User data to pass to the callback. Can be NULL.
 *
 * @note Fires an assert if @p cb is NULL. Calling this function with @p cb equal to NULL would be equivalent to not
 * calling this function at all.
 */
void linked_list_remove_on_condition(LinkedList self, LinkedListConditionCb cb, void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_UTILS_LINKED_LIST_H */
