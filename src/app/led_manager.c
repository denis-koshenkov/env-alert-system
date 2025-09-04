#include <stddef.h>
#include <stdbool.h>

#include "led_manager.h"
#include "hal/hw_platform.h"
#include "eas_timer.h"
#include "config.h"

#ifndef CONFIG_LED_MANAGER_NOTIFICATION_DURATION_SECONDS
#define CONFIG_LED_MANAGER_NOTIFICATION_DURATION_SECONDS 5
#endif

#define LED_MANAGER_NOTIFICATION_DURATION_MS (CONFIG_LED_MANAGER_NOTIFICATION_DURATION_SECONDS * 1000)

#define LED_MANAGER_MAX_NUM_NOTIFICATIONS 3

typedef struct LedNotification {
    /**< Defined as uint8_t to save memory, use values from enum LedColor. */
    uint8_t led_color;
    /**< Defined as uint8_t to save memory, use values from enum LedPattern. */
    uint8_t led_pattern;
} LedNotification;

static size_t num_notifications = 0;

static LedNotification led_notifications[LED_MANAGER_MAX_NUM_NOTIFICATIONS];

static LedColor led_color_0;
static LedPattern led_pattern_0;
static LedColor led_color_1;
static LedPattern led_pattern_1;

static void notification_timer_cb(void *user_data)
{
    static size_t notification_idx = 1;
    hw_platform_get_led()->set(led_notifications[notification_idx].led_color,
                               led_notifications[notification_idx].led_pattern);
    notification_idx = (notification_idx + 1) % num_notifications;
}

static EasTimer get_timer_instance()
{
    static EasTimer instance;
    static bool is_created = false;
    if (!is_created) {
        instance =
            eas_timer_create(LED_MANAGER_NOTIFICATION_DURATION_MS, EAS_TIMER_PERIODIC, notification_timer_cb, NULL);
        is_created = true;
    }
    return instance;
}

void led_manager_add_notification(LedColor led_color, LedPattern led_pattern)
{
    if (num_notifications == 0) {
        /* This is the only notification - set the led immediately */
        hw_platform_get_led()->set(led_color, led_pattern);
        led_notifications[0].led_color = led_color;
        led_notifications[0].led_pattern = led_pattern;
    } else if (num_notifications == 1) {
        eas_timer_start(get_timer_instance());
        led_notifications[1].led_color = led_color;
        led_notifications[1].led_pattern = led_pattern;
    } else if (num_notifications == 2) {
        led_notifications[2].led_color = led_color;
        led_notifications[2].led_pattern = led_pattern;
    }
    num_notifications++;
}

void led_manager_remove_notification(LedColor led_color, LedPattern led_pattern)
{
    if (num_notifications == 1) {
        /* This is the last notification, and it is being removed. Turn off the led - no notifications to display. */
        hw_platform_get_led()->turn_off(led_color, led_pattern);
    } else if (num_notifications == 2) {
        /* The second to last notification is being removed. Since only one notification is left, we do not need to
         * alternate between the notification - stop the notification timer. */
        eas_timer_stop(get_timer_instance());
    }
    num_notifications--;
}
