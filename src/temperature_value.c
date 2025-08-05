#include <stddef.h>

#include "config.h"
#include "eas_assert.h"
#include "temperature_value.h"
#include "value_holder.h"

#ifndef CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES
#define CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES 1
#endif

struct temperature_value_struct {
    value_holder value_holder;
    temperature value_buf;
};

static struct temperature_value_struct instances[CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

temperature_value temperature_value_create()
{
    EAS_ASSERT(instance_idx < CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES);
    struct temperature_value_struct *instance = &instances[instance_idx];
    instance_idx++;

    instance->value_holder = value_holder_create((uint8_t *)&instance->value_buf, sizeof(temperature));
    return instance;
}

void temperature_value_set(temperature_value tv, temperature temperature)
{
    value_holder_set(tv->value_holder, &temperature);
}

temperature temperature_value_get(temperature_value tv)
{
    temperature temperature;
    value_holder_get(NULL, &temperature);
    return temperature;
}
