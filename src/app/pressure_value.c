#include <stddef.h>

#include "config.h"
#include "eas_assert.h"
#include "pressure_value.h"
#include "value_holder.h"

#ifndef CONFIG_PRESSURE_VALUE_MAX_NUM_INSTANCES
#define CONFIG_PRESSURE_VALUE_MAX_NUM_INSTANCES 1
#endif

struct PressureValueStruct {
    ValueHolder value_holder;
    Pressure value_buf;
};

static struct PressureValueStruct instances[CONFIG_PRESSURE_VALUE_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

PressureValue pressure_value_create()
{
    EAS_ASSERT(instance_idx < CONFIG_PRESSURE_VALUE_MAX_NUM_INSTANCES);
    struct PressureValueStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->value_holder = value_holder_create((uint8_t *)&instance->value_buf, sizeof(Pressure));
    return instance;
}

void pressure_value_set(PressureValue pv, Pressure pressure)
{
    value_holder_set(pv->value_holder, &pressure);
}

Pressure pressure_value_get(PressureValue pv)
{
    Pressure pressure;
    value_holder_get(pv->value_holder, &pressure);
    return pressure;
}

bool pressure_value_is_value_changed(PressureValue pv)
{
    return value_holder_is_value_changed(pv->value_holder);
}
