#include <stdbool.h>

#include "CppUTestExt/MockSupport.h"
#include "mock_pressure_value.h"

struct PressureValueStruct {};

PressureValue pressure_value_create()
{
    mock().actualCall("pressure_value_create");
    return (PressureValue)mock().pointerReturnValue();
}

void pressure_value_set(PressureValue pv, Pressure pressure)
{
    mock().actualCall("pressure_value_set").withParameter("pv", pv).withParameter("pressure", pressure);
}

Pressure pressure_value_get(PressureValue pv)
{
    mock().actualCall("pressure_value_get").withParameter("pv", pv);
    return mock().unsignedIntReturnValue();
}

bool pressure_value_is_value_changed(PressureValue pv)
{
    mock().actualCall("pressure_value_is_value_changed").withParameter("pv", pv);
    return mock().boolReturnValue();
}
