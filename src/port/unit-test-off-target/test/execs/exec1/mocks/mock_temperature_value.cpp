#include <stdbool.h>

#include "CppUTestExt/MockSupport.h"
#include "mock_temperature_value.h"

struct TemperatureValueStruct {};

TemperatureValue temperature_value_create()
{
    mock().actualCall("temperature_value_create");
    return (TemperatureValue)mock().pointerReturnValue();
}

void temperature_value_set(TemperatureValue tv, Temperature temperature)
{
    mock().actualCall("temperature_value_set").withParameter("tv", tv).withParameter("temperature", temperature);
}

Temperature temperature_value_get(TemperatureValue tv)
{
    mock().actualCall("temperature_value_get").withParameter("tv", tv);
    return mock().unsignedIntReturnValue();
}

bool temperature_value_is_value_changed(TemperatureValue tv)
{
    mock().actualCall("temperature_value_is_value_changed").withParameter("tv", tv);
    return mock().boolReturnValue();
}
