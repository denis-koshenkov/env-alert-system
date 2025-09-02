#include "CppUTestExt/MockSupport.h"
#include "mock_alert_raiser.h"

struct AlertRaiserStruct {};

AlertRaiser alert_raiser_create()
{
    mock().actualCall("alert_raiser_create");
    return (AlertRaiser)mock().pointerReturnValue();
}
