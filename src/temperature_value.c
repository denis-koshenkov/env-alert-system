#include <stddef.h>

#include "config.h"
#include "eas_assert.h"
#include "temperature_value.h"

#ifndef CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES
#define CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES 1
#endif

struct temperature_value_struct {};

static struct temperature_value_struct instances[CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

temperature_value temperature_value_create()
{
    EAS_ASSERT(instance_idx < CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES);
    struct temperature_value_struct *instance = &instances[instance_idx];
    instance_idx++;

    return instance;
}

void temperature_value_set(temperature_value tv, temperature t)
{
}

temperature temperature_value_get(temperature_value tv)
{
    return 22;
}
