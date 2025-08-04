#ifndef VALUE_HOLDER_H
#define VALUE_HOLDER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>

typedef struct value_holder_struct *value_holder;

value_holder value_holder_create(uint8_t *value_buf, size_t value_size);

#ifdef __cplusplus
}
#endif

#endif
