#include <stdbool.h>

#include "CppUTestExt/MockSupport.h"
#include "mock_current_light_intensity.h"

void current_light_intensity_set(LightIntensity light_intensity)
{
    mock().actualCall("current_light_intensity_set").withParameter("light_intensity", light_intensity);
}

LightIntensity current_light_intensity_get()
{
    mock().actualCall("current_light_intensity_get");
    return mock().unsignedIntReturnValue();
}

bool current_light_intensity_is_changed()
{
    mock().actualCall("current_light_intensity_is_changed");
    return mock().boolReturnValue();
}
