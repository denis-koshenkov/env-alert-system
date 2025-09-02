#include "connectivity_notifier.h"
#include "connectivity_notification_sender.h"
#include "config.h"
#include "eas_assert.h"

#ifndef CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS
#define CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS 1
#endif

static bool registered_notifications[CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS];

static bool is_valid_alert_id(uint8_t alert_id)
{
    return (alert_id < CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS);
}

void connectivity_notifier_register_notification(uint8_t alert_id)
{
    EAS_ASSERT(is_valid_alert_id(alert_id));

    registered_notifications[alert_id] = true;
}

void connectivity_notifier_unregister_notification(uint8_t alert_id)
{
    EAS_ASSERT(is_valid_alert_id(alert_id));

    registered_notifications[alert_id] = false;
}

void connectivity_notifier_notify(uint8_t alert_id, bool is_raised)
{
    EAS_ASSERT(is_valid_alert_id(alert_id));

    if (registered_notifications[alert_id]) {
        connectivity_notification_sender_send(alert_id, is_raised);
    }
}
