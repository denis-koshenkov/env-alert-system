#include "stdbool.h"

#include "current_light_intensity.h"
#include "light_intensity_value.h"

static LightIntensityValue get_instance()
{
    static LightIntensityValue instance;
    static bool is_created = false;
    if (!is_created) {
        instance = light_intensity_value_create();
        is_created = true;
    }
    return instance;
}

void current_light_intensity_set(LightIntensity light_intensity)
{
    light_intensity_value_set(get_instance(), light_intensity);
}

LightIntensity current_light_intensity_get()
{
    return light_intensity_value_get(get_instance());
}

bool current_light_intensity_is_changed()
{
    return light_intensity_value_is_value_changed(get_instance());
}
