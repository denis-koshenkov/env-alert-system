#include "connectivity_notifier.h"
#include "connectivity_notification_sender.h"
#include "config.h"

#ifndef CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS
#define CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS 1
#endif

static bool is_notification_registered = false;

void connectivity_notifier_register_notification(uint8_t alert_id)
{
    is_notification_registered = true;
}

void connectivity_notifier_notify(uint8_t alert_id, bool is_raised)
{
    if (is_notification_registered) {
        connectivity_notification_sender_send(0, true);
    }
}
