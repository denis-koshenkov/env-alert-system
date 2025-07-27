#include <stddef.h>

#include "config.h"
#include "eas_assert.h"
#include "temperature_value.h"

#ifndef CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES
#define CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES 1
#endif

struct temperature_value_struct {
    temperature value;
    bool set_has_been_called;
};

static struct temperature_value_struct instances[CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

temperature_value temperature_value_create()
{
    EAS_ASSERT(instance_idx < CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES);
    struct temperature_value_struct *instance = &instances[instance_idx];
    instance_idx++;

    instance->set_has_been_called = false;
    return instance;
}

void temperature_value_set(temperature_value t, temperature value)
{
    EAS_ASSERT(t);
    t->value = value;
    t->set_has_been_called = true;
}

temperature temperature_value_get(temperature_value t)
{
    EAS_ASSERT(t);
    return t->value;
}

bool temperature_value_is_value_changed(temperature_value t)
{
    EAS_ASSERT(t->set_has_been_called);
    return true;
}
