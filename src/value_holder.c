#include <string.h>

#include "config.h"
#include "eas_assert.h"
#include "value_holder.h"

#ifndef CONFIG_VALUE_HOLDER_MAX_NUM_INSTANCES
#define CONFIG_VALUE_HOLDER_MAX_NUM_INSTANCES 1
#endif

struct value_holder_struct {
    uint8_t *value_buf;
    size_t value_size;
};

static struct value_holder_struct instances[CONFIG_VALUE_HOLDER_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

value_holder value_holder_create(uint8_t *value_buf, size_t value_size)
{
    EAS_ASSERT(instance_idx < CONFIG_VALUE_HOLDER_MAX_NUM_INSTANCES);
    struct value_holder_struct *instance = &instances[instance_idx];
    instance_idx++;

    instance->value_buf = value_buf;
    instance->value_size = value_size;

    return instance;
}

void value_holder_set(value_holder t, void *value)
{
    memcpy(t->value_buf, value, t->value_size);
}

void value_holder_get(value_holder t, void *value)
{
    memcpy(value, t->value_buf, t->value_size);
}
