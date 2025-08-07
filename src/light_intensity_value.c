#include <stddef.h>

#include "config.h"
#include "eas_assert.h"
#include "light_intensity_value.h"
#include "value_holder.h"

#ifndef CONFIG_LIGHT_INTENSITY_VALUE_MAX_NUM_INSTANCES
#define CONFIG_LIGHT_INTENSITY_VALUE_MAX_NUM_INSTANCES 1
#endif

struct light_intensity_value_struct {
    value_holder value_holder;
    light_intensity value_buf;
};

static struct light_intensity_value_struct instances[CONFIG_LIGHT_INTENSITY_VALUE_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

light_intensity_value light_intensity_value_create()
{
    EAS_ASSERT(instance_idx < CONFIG_LIGHT_INTENSITY_VALUE_MAX_NUM_INSTANCES);
    struct light_intensity_value_struct *instance = &instances[instance_idx];
    instance_idx++;

    instance->value_holder = value_holder_create((uint8_t *)&instance->value_buf, sizeof(light_intensity));
    return instance;
}

void light_intensity_value_set(light_intensity_value liv, light_intensity light_intensity)
{
    value_holder_set(liv->value_holder, &light_intensity);
}

light_intensity light_intensity_value_get(light_intensity_value liv)
{
    light_intensity light_intensity;
    value_holder_get(liv->value_holder, &light_intensity);
    return light_intensity;
}

bool light_intensity_value_is_value_changed(light_intensity_value liv)
{
    return value_holder_is_value_changed(liv->value_holder);
}
