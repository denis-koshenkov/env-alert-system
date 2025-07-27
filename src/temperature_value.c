#include <stddef.h>

#include "config.h"
#include "eas_assert.h"
#include "temperature_value.h"

#ifndef CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES
#define CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES 1
#endif

struct temperature_value_struct {
    temperature value;
};

static struct temperature_value_struct instances[CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

temperature_value temperature_value_create()
{
    EAS_ASSERT(instance_idx < CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES);
    return &instances[instance_idx++];
}

void temperature_value_set(temperature_value t, temperature value)
{
    t->value = value;
}

temperature temperature_value_get(temperature_value t)
{
    return t->value;
}
