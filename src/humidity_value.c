#include <stddef.h>

#include "config.h"
#include "eas_assert.h"
#include "humidity_value.h"
#include "value_holder.h"

#ifndef CONFIG_HUMIDITY_VALUE_MAX_NUM_INSTANCES
#define CONFIG_HUMIDITY_VALUE_MAX_NUM_INSTANCES 1
#endif

struct HumidityValueStruct {
    ValueHolder value_holder;
    Humidity value_buf;
};

static struct HumidityValueStruct instances[CONFIG_HUMIDITY_VALUE_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

HumidityValue humidity_value_create()
{
    EAS_ASSERT(instance_idx < CONFIG_HUMIDITY_VALUE_MAX_NUM_INSTANCES);
    struct HumidityValueStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->value_holder = value_holder_create((uint8_t *)&instance->value_buf, sizeof(Humidity));
    return instance;
}

void humidity_value_set(HumidityValue hv, Humidity humidity)
{
    value_holder_set(hv->value_holder, &humidity);
}

Humidity humidity_value_get(HumidityValue hv)
{
    Humidity humidity;
    value_holder_get(hv->value_holder, &humidity);
    return humidity;
}

bool humidity_value_is_value_changed(HumidityValue hv)
{
    return value_holder_is_value_changed(hv->value_holder);
}
