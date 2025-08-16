#include <stddef.h>

#include "config.h"
#include "eas_assert.h"
#include "temperature_value.h"
#include "value_holder.h"

#ifndef CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES
#define CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES 1
#endif

struct TemperatureValueStruct {
    value_holder value_holder;
    Temperature value_buf;
};

static struct TemperatureValueStruct instances[CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

TemperatureValue temperature_value_create()
{
    EAS_ASSERT(instance_idx < CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES);
    struct TemperatureValueStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->value_holder = value_holder_create((uint8_t *)&instance->value_buf, sizeof(Temperature));
    return instance;
}

void temperature_value_set(TemperatureValue tv, Temperature temperature)
{
    value_holder_set(tv->value_holder, &temperature);
}

Temperature temperature_value_get(TemperatureValue tv)
{
    Temperature temperature;
    value_holder_get(tv->value_holder, &temperature);
    return temperature;
}

bool temperature_value_is_value_changed(TemperatureValue tv)
{
    return value_holder_is_value_changed(tv->value_holder);
}
