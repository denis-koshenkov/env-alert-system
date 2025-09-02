#include "connectivity_notification_sender.h"

void connectivity_notification_sender_send(uint8_t alert_id, bool is_raised)
{
    /* Empty for now, because unit tests mock this interface instead of using the real one (this one). The
     * implementation here should call a connectivity protocol API to actually transmit the notification message. */
}
