#include "connectivity_notification_sender.h"
#include "msg_transceiver.h"
#include "eas_log.h"

EAS_LOG_ENABLE_IN_FILE();

void connectivity_notification_sender_send(uint8_t alert_id, bool is_raised)
{
    EAS_LOG_INF("Sending connectivity notification for alert id %u, raised: %d", alert_id, is_raised);
    msg_transceiver_send_alert_status_change_message(alert_id, is_raised, NULL, NULL);
}
