#include <stdbool.h>

#include "CppUTestExt/MockSupport.h"
#include "mock_current_temperature.h"

void current_temperature_set(Temperature temperature)
{
    mock().actualCall("current_temperature_set").withParameter("temperature", temperature);
}

Temperature current_temperature_get()
{
    mock().actualCall("current_temperature_get");
    return mock().unsignedIntReturnValue();
}

bool current_temperature_is_changed()
{
    mock().actualCall("current_temperature_is_changed");
    return mock().boolReturnValue();
}
