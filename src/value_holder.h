#ifndef VALUE_HOLDER_H
#define VALUE_HOLDER_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Stores a value and reports when it changes.
 *
 * Contract:
 *   1. get() should be called only after set() has been called at least once.
 */
typedef struct value_holder_struct *value_holder;

value_holder value_holder_create(uint8_t *value_buf, size_t value_size);

void value_holder_set(value_holder vh, const void *value);

void value_holder_get(value_holder vh, void *value);

/* TODO: write that it returns true after the first time set() is called, regardless of the value. */
bool value_holder_is_value_changed(value_holder vh);

#endif
