#include "led_notifier.h"
#include "led_notification_executor.h"
#include "config.h"
#include "eas_assert.h"

#ifndef CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS
#define CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS 1
#endif

typedef struct AlertLedNotification {
    /**< Use values from LedColor enum, defined as uint8_t to save memory. */
    uint8_t led_color;
    /**< Use values from LedPattern enum, defined as uint8_t to save memory. */
    uint8_t led_pattern;
    /**< Whether led notification is enabled for this alert. */
    bool is_enabled;
} AlertLedNotification;

static AlertLedNotification alert_led_notifications[CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS];

static bool is_valid_alert_id(uint8_t alert_id)
{
    return (alert_id < CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS);
}

void led_notifier_notify(uint8_t alert_id, bool is_raised)
{
    EAS_ASSERT(is_valid_alert_id(alert_id));

    if (alert_led_notifications[alert_id].is_enabled) {
        led_notification_executor_execute(alert_led_notifications[alert_id].led_color,
                                          alert_led_notifications[alert_id].led_pattern, is_raised);
    }
}

void led_notifier_enable_notifications(uint8_t alert_id, LedColor led_color, LedPattern led_pattern)
{
    EAS_ASSERT(is_valid_alert_id(alert_id));

    alert_led_notifications[alert_id].led_color = led_color;
    alert_led_notifications[alert_id].led_pattern = led_pattern;
    alert_led_notifications[alert_id].is_enabled = true;
}

void led_notifier_disable_notifications(uint8_t alert_id)
{
    EAS_ASSERT(is_valid_alert_id(alert_id));

    alert_led_notifications[alert_id].is_enabled = false;
}
