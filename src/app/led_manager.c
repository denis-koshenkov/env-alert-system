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

static size_t num_notifications = 0;

static void notification_timer_cb(void *user_data)
{
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
    } else {
        eas_timer_start(get_timer_instance());
    }
    num_notifications++;
}

void led_manager_remove_notification(LedColor led_color, LedPattern led_pattern)
{
    if (num_notifications == 1) {
        /* This is the last notification, and it is being removed. Turn off the led - no notifications to display. */
        hw_platform_get_led()->turn_off(led_color, led_pattern);
    }
    num_notifications--;
}
