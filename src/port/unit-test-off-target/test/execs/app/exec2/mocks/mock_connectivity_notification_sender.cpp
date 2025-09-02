#include "CppUTestExt/MockSupport.h"
#include "mock_connectivity_notification_sender.h"

void connectivity_notification_sender_send_notification(uint8_t alert_id, bool is_raised)
{
    mock()
        .actualCall("connectivity_notification_sender_send_notification")
        .withParameter("alert_id", alert_id)
        .withParameter("is_raised", is_raised);
}
