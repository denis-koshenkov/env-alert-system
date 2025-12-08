#include "connectivity_notification_sender.h"
#include "msg_transceiver.h"

void connectivity_notification_sender_send(uint8_t alert_id, bool is_raised)
{
    msg_transceiver_send_alert_status_change_message(alert_id, is_raised, NULL, NULL);
}
