#include "led_notifier.h"
#include "led_notification_executor.h"
#include "config.h"

#ifndef CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS
#define CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS 1
#endif

typedef struct AlertLedNotification {
    /**< Use values from LedColor enum, defined as uint8_t to save memory. */
    uint8_t led_color;
    /**< Use values from LedPattern enum, defined as uint8_t to save memory. */
    uint8_t led_pattern;
} AlertLedNotification;

static bool notifications_enabled = false;
static AlertLedNotification alert_led_notification;

void led_notifier_notify(uint8_t alert_id, bool is_raised)
{
    if (notifications_enabled) {
        led_notification_executor_execute(alert_led_notification.led_color, alert_led_notification.led_pattern, true);
    }
}

void led_notifier_enable_notifications(uint8_t alert_id, LedColor led_color, LedPattern led_pattern)
{
    alert_led_notification.led_color = led_color;
    alert_led_notification.led_pattern = led_pattern;
    notifications_enabled = true;
}

void led_notifier_disable_notifications(uint8_t alert_id)
{
}
