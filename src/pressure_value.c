#include <stddef.h>

#include "config.h"
#include "eas_assert.h"
#include "pressure_value.h"
#include "value_holder.h"

#ifndef CONFIG_PRESSURE_VALUE_MAX_NUM_INSTANCES
#define CONFIG_PRESSURE_VALUE_MAX_NUM_INSTANCES 1
#endif

struct pressure_value_struct {
    value_holder value_holder;
    pressure value_buf;
};

static struct pressure_value_struct instances[CONFIG_PRESSURE_VALUE_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

pressure_value pressure_value_create()
{
    EAS_ASSERT(instance_idx < CONFIG_PRESSURE_VALUE_MAX_NUM_INSTANCES);
    struct pressure_value_struct *instance = &instances[instance_idx];
    instance_idx++;

    instance->value_holder = value_holder_create((uint8_t *)&instance->value_buf, sizeof(pressure));
    return instance;
}

void pressure_value_set(pressure_value pv, pressure pressure)
{
    value_holder_set(pv->value_holder, &pressure);
}

pressure pressure_value_get(pressure_value pv)
{
    pressure pressure;
    value_holder_get(pv->value_holder, &pressure);
    return pressure;
}

bool pressure_value_is_value_changed(pressure_value pv)
{
    return value_holder_is_value_changed(pv->value_holder);
}
