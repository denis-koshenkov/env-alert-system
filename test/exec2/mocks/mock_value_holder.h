#ifndef VALUE_HOLDER_H
#define VALUE_HOLDER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>

typedef struct ValueHolderStruct *ValueHolder;

ValueHolder value_holder_create(uint8_t *value_buf, size_t value_size);

void value_holder_set(ValueHolder vh, const void *value);

void value_holder_get(ValueHolder vh, void *value);

bool value_holder_is_value_changed(ValueHolder vh);

#ifdef __cplusplus
}
#endif

#endif
