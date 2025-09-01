#include "CppUTestExt/MockSupport.h"
#include "mock_alert_notifier.h"

void alert_notifier_notify(uint8_t alert_id, bool is_raised)
{
    mock()
        .actualCall("alert_notifier_notify")
        .withParameter("alert_id", alert_id)
        .withParameter("is_raised", is_raised);
}
