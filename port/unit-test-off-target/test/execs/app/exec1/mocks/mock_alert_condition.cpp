#include "CppUTestExt/MockSupport.h"
#include "mock_alert_condition.h"

struct AlertConditionStruct {};

AlertCondition alert_condition_create()
{
    mock().actualCall("alert_condition_create");
    return (AlertCondition)mock().pointerReturnValue();
}
