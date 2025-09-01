#include <stddef.h>

#include "config.h"
#include "eas_assert.h"
#include "light_intensity_value.h"
#include "value_holder.h"

#ifndef CONFIG_LIGHT_INTENSITY_VALUE_MAX_NUM_INSTANCES
#define CONFIG_LIGHT_INTENSITY_VALUE_MAX_NUM_INSTANCES 1
#endif

struct LightIntensityValueStruct {
    ValueHolder value_holder;
    LightIntensity value_buf;
};

static struct LightIntensityValueStruct instances[CONFIG_LIGHT_INTENSITY_VALUE_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

LightIntensityValue light_intensity_value_create()
{
    EAS_ASSERT(instance_idx < CONFIG_LIGHT_INTENSITY_VALUE_MAX_NUM_INSTANCES);
    struct LightIntensityValueStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->value_holder = value_holder_create((uint8_t *)&instance->value_buf, sizeof(LightIntensity));
    return instance;
}

void light_intensity_value_set(LightIntensityValue liv, LightIntensity light_intensity)
{
    value_holder_set(liv->value_holder, &light_intensity);
}

LightIntensity light_intensity_value_get(LightIntensityValue liv)
{
    LightIntensity light_intensity;
    value_holder_get(liv->value_holder, &light_intensity);
    return light_intensity;
}

bool light_intensity_value_is_value_changed(LightIntensityValue liv)
{
    return value_holder_is_value_changed(liv->value_holder);
}
