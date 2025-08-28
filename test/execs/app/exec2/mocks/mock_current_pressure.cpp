#include <stdbool.h>

#include "CppUTestExt/MockSupport.h"
#include "mock_current_pressure.h"

void current_pressure_set(Pressure pressure)
{
    mock().actualCall("current_pressure_set").withParameter("pressure", pressure);
}

Pressure current_pressure_get()
{
    mock().actualCall("current_pressure_get");
    return mock().unsignedIntReturnValue();
}

bool current_pressure_is_changed()
{
    mock().actualCall("current_pressure_is_changed");
    return mock().boolReturnValue();
}
