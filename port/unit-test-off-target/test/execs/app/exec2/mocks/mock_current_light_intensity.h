#ifndef MOCK_CURRENT_LIGHT_INTENSITY_H
#define MOCK_CURRENT_LIGHT_INTENSITY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "hal/light_intensity.h"

void current_light_intensity_set(LightIntensity light_intensity);

LightIntensity current_light_intensity_get();

bool current_light_intensity_is_changed();

#ifdef __cplusplus
}
#endif

#endif
