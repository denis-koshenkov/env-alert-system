#ifndef ENV_ALERT_SYSTEM_SRC_APP_UTILS_LINKED_LIST_H
#define ENV_ALERT_SYSTEM_SRC_APP_UTILS_LINKED_LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#define LINKED_LIST_REMOVE_NO_LIMIT SIZE_MAX

/**
 * @brief Singly linked list.
 *
 * It is possible to iterate over all the elements in the linked list by using @ref linked_list_iterator_init and @ref
 * linked_list_iterator_next. Usage:
 *
 * ```
 * // Create a linked list and add some elements
 * size_t element0 = 42;
 * size_t element1 = 24;
 * LinkedList linked_list = linked_list_create();
 * linked_list_prepend(linked_list, &element1);
 * linked_list_prepend(linked_list, &element0);
 *
 * // Initialize the iterator
 * void *iterator = linked_list_iterator_init();
 *
 * // This will set element0_retrieved to the address of element0 passed to linked_list_prepend
 * // is_valid_element0 will be true, since linked_list_iterator_next retrieved a valid element
 * size_t *element0_retrieved;
 * bool is_valid_element0 = linked_list_iterator_next(&iterator, (void **)&element0_retrieved);
 *
 * // This will set element1_retrieved to the address of element1 passed to linked_list_prepend
 * // is_valid_element1 will be true, since linked_list_iterator_next retrieved a valid element
 * size_t *element1_retrieved;
 * bool is_valid_element1 = linked_list_iterator_next(&iterator, (void **)&element1_retrieved);
 *
 * // We already iterated over all elements in the list. There is no next element to retrieve. is_valid_element2 will
 * // be false. element2_retrieved has undefined value.
 * size_t *element2_retrieved;
 * bool is_valid_element2 = linked_list_iterator_next(&iterator, (void **)&element2_retrieved);
 * ```
 *
 * If an element is added to the list or removed from the list after @ref linked_list_iterator_init is called, that
 * iterator should no longer be used. It is not allowed to call @ref linked_list_iterator_next with an iterator that was
 * initialized before elements were added or removed. If elements are added or removed, a new iterator needs to be
 * initialized and only then passed to @ref linked_list_iterator_next. The iteration will then start from the first
 * element in the list.
 *
 * The reason for this is that the iterator is implemented as a pointer to a linked list node. If that node is removed
 * from the list, the iterator is pointing to already freed memory.
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
 * @brief Callback type to execute for each element before removing it.
 *
 * @param element Element that is about to be removed.
 * @param user_data User data.
 */
typedef void (*LinkedListPreRemoveCb)(void *element, void *user_data);

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
 * @brief Add an element to the end of the linked list.
 *
 * Less efficient than @ref linked_list_prepend, because the implementation needs to iterate till the end of the list.
 * Use @ref linked_list_prepend if the order of elements in the list does not matter.
 *
 * @param self Linked list instance returned by @ref linked_list_create.
 * @param element Element to add to the list.
 */
void linked_list_append(LinkedList self, void *element);

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
 * Equivalent to calling:
 * linked_list_remove_on_condition_with_limit(self, LINKED_LIST_REMOVE_NO_LIMIT, cb, user_data, NULL, NULL);
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

/**
 * @brief Remove all elements from the list that satisfy a certain condition with limit and pre remove callback.
 *
 * @param self Linked list instance returned by @ref linked_list_create.
 * @param max_num_elements_to_remove Maximal number of elements to remove. If the implementation removes this number
 * of elements from the list, it exits immediately without iterating through the rest of the list. Pass
 * LINKED_LIST_REMOVE_NO_LIMIT if all elements that satisfy the condition should be removed from the list, regardless of
 * how many.
 * @param condition_cb This callback is called for each element in the list. If the callback for the element returns
 * true, then the element is removed from the list. If the callback for the element returns false, the element is kept
 * in the list.
 * @param condition_cb_user_data User data to pass to the condition callback. Can be NULL.
 * @param pre_remove_cb Optional callback that is called for each element that is removed from the list right before the
 * element is removed. Can be NULL.
 * @param pre_remove_cb_user_data User data to pass to the pre remove callback. Can be NULL.
 *
 * @return size_t Number of elements removed from the list.
 *
 * @note Fires an assert if @p condition_cb is NULL. Calling this function with @p condition_cb equal to NULL would be
 * equivalent to not calling this function at all.
 */
size_t linked_list_remove_on_condition_with_limit(LinkedList self, size_t max_num_elements_to_remove,
                                                  LinkedListConditionCb condition_cb, void *condition_cb_user_data,
                                                  LinkedListPreRemoveCb pre_remove_cb, void *pre_remove_cb_user_data);

/**
 * @brief Initialize a linked list iterator.
 *
 * @param self Linked list instance returned by @ref linked_list_create.
 *
 * @return void* Initialized iterator.
 */
void *linked_list_iterator_init(LinkedList self);

/**
 * @brief Retrieve the next element in the list using the iterator.
 *
 * @param iterator Address of the iterator returned by @ref linked_list_iterator_init.
 * @param[out] element Address of the current retrieved element is written to this parameter. This only happens if true
 * is returned. If this function returns false, this parameter is considered to have an undefined value and should not
 * be dereferenced.
 *
 * @return true The next element was successfully retrieved and written to @p element.
 * @return false There is no next element to retrieve - the end of the list has been reached.
 *
 * @warning It is not allowed to call this function if after @p iterator was initialized, elements were added to or
 * removed from the list. If that happens, a new iterator needs to be initialized and the iteration has to start from
 * the beginning of the list.
 */
bool linked_list_iterator_next(void **iterator, void **element);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_UTILS_LINKED_LIST_H */
