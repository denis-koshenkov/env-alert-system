#include <stdbool.h>

#include "CppUTestExt/MockSupport.h"
#include "mock_humidity_value.h"

struct HumidityValueStruct {};

HumidityValue humidity_value_create()
{
    mock().actualCall("humidity_value_create");
    return (HumidityValue)mock().pointerReturnValue();
}

void humidity_value_set(HumidityValue hv, Humidity humidity)
{
    mock().actualCall("humidity_value_set").withParameter("hv", hv).withParameter("humidity", humidity);
}

Humidity humidity_value_get(HumidityValue hv)
{
    mock().actualCall("humidity_value_get").withParameter("hv", hv);
    return mock().unsignedIntReturnValue();
}

bool humidity_value_is_value_changed(HumidityValue hv)
{
    mock().actualCall("humidity_value_is_value_changed").withParameter("hv", hv);
    return mock().boolReturnValue();
}
