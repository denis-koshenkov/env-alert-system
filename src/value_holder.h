#ifndef VALUE_HOLDER_H
#define VALUE_HOLDER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Stores a value and reports when it changes.
 */
typedef struct value_holder_struct *value_holder;

/**
 * @brief Create a value holder instance.
 *
 * @param value_buf Buffer of size @p value_size bytes. This buffer is used internally by this module to store
 * the value. This buffer should exist for as long as the instance of value_holder is being used.
 * @param value_size Value size in bytes.
 *
 * @return value_holder Value holder instance.
 */
value_holder value_holder_create(uint8_t *value_buf, size_t value_size);

/**
 * @brief Set value.
 *
 * @param vh Value holder instance returned by @ref value_holder_create.
 * @param value Pointer to value to be set. Value should be of the same size as the value_size argument
 * passed to @ref value_holder_create.
 */
void value_holder_set(value_holder vh, const void *value);

/**
 * @brief Get the last set value.
 *
 * @pre @ref value_holder_set has been called at least once.
 *
 * @param vh Value holder instance returned by @ref value_holder_create.
 * @param[out] value Value that was last passed to the value holder instance using @ref value_holder_set.
 */
void value_holder_get(value_holder vh, void *value);

/**
 * @brief Check whether the value has changed.
 *
 * @pre @ref value_holder_set has been called at least once.
 *
 * @param vh Value holder instance returned by @ref value_holder_create.
 *
 * @return true The last two calls to @ref value_holder_set had the same value as arguments, or @ref value_holder_set
 * has been called only once so far.
 * @return false The last two calls to @ref value_holder_set had differing values as arguments.
 */
bool value_holder_is_value_changed(value_holder vh);

#ifdef __cplusplus
}
#endif

#endif
