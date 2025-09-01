#include <stdbool.h>

#include "CppUTestExt/MockSupport.h"
#include "mock_current_humidity.h"

void current_humidity_set(Humidity humidity)
{
    mock().actualCall("current_humidity_set").withParameter("humidity", humidity);
}

Humidity current_humidity_get()
{
    mock().actualCall("current_humidity_get");
    return mock().unsignedIntReturnValue();
}

bool current_humidity_is_changed()
{
    mock().actualCall("current_humidity_is_changed");
    return mock().boolReturnValue();
}
