#include <stdbool.h>
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
    bool set_has_been_called;
    bool value_changed;
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
    instance->set_has_been_called = false;
    instance->value_changed = false;

    return instance;
}

void value_holder_set(value_holder vh, const void *value)
{
    EAS_ASSERT(vh);
    EAS_ASSERT(value);

    if (vh->set_has_been_called) {
        vh->value_changed = memcmp(value, vh->value_buf, vh->value_size);
    } else {
        /* Value is being set for the first time ever -> it has changed */
        vh->value_changed = true;
    }

    memcpy(vh->value_buf, value, vh->value_size);
    vh->set_has_been_called = true;
}

void value_holder_get(value_holder vh, void *value)
{
    EAS_ASSERT(vh);
    EAS_ASSERT(value);
    EAS_ASSERT(vh->set_has_been_called);
    memcpy(value, vh->value_buf, vh->value_size);
}

bool value_holder_is_value_changed(value_holder vh)
{
    EAS_ASSERT(vh);
    EAS_ASSERT(vh->set_has_been_called);
    return vh->value_changed;
}
