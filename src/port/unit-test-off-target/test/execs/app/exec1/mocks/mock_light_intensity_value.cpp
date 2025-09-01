#include <stdbool.h>

#include "CppUTestExt/MockSupport.h"
#include "mock_light_intensity_value.h"

struct LightIntensityValueStruct {};

LightIntensityValue light_intensity_value_create()
{
    mock().actualCall("light_intensity_value_create");
    return (LightIntensityValue)mock().pointerReturnValue();
}

void light_intensity_value_set(LightIntensityValue liv, LightIntensity light_intensity)
{
    mock()
        .actualCall("light_intensity_value_set")
        .withParameter("liv", liv)
        .withParameter("light_intensity", light_intensity);
}

LightIntensity light_intensity_value_get(LightIntensityValue liv)
{
    mock().actualCall("light_intensity_value_get").withParameter("liv", liv);
    return mock().unsignedIntReturnValue();
}

bool light_intensity_value_is_value_changed(LightIntensityValue liv)
{
    mock().actualCall("light_intensity_value_is_value_changed").withParameter("liv", liv);
    return mock().boolReturnValue();
}
