#include <stddef.h>

#include "config.h"
#include "eas_assert.h"
#include "humidity_value.h"
#include "value_holder.h"

#ifndef CONFIG_HUMIDITY_VALUE_MAX_NUM_INSTANCES
#define CONFIG_HUMIDITY_VALUE_MAX_NUM_INSTANCES 1
#endif

struct humidity_value_struct {
    value_holder value_holder;
    humidity value_buf;
};

static struct humidity_value_struct instances[CONFIG_HUMIDITY_VALUE_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

humidity_value humidity_value_create()
{
    EAS_ASSERT(instance_idx < CONFIG_HUMIDITY_VALUE_MAX_NUM_INSTANCES);
    struct humidity_value_struct *instance = &instances[instance_idx];
    instance_idx++;

    instance->value_holder = value_holder_create((uint8_t *)&instance->value_buf, sizeof(humidity));
    return instance;
}

void humidity_value_set(humidity_value hv, humidity humidity)
{
    value_holder_set(hv->value_holder, &humidity);
}

humidity humidity_value_get(humidity_value hv)
{
    humidity humidity;
    value_holder_get(hv->value_holder, &humidity);
    return humidity;
}

bool humidity_value_is_value_changed(humidity_value hv)
{
    return value_holder_is_value_changed(hv->value_holder);
}
