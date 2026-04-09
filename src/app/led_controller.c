#include "led_controller.h"
#include "hw_platform.h"
#include "eas_timer.h"
#include "eas_assert.h"

#define LED_CONTROLLER_ALERT_PATTERN_TIMER_PERIOD_MS 300

typedef struct {
    void (*start)(LedColor color);
    void (*stop)();
} LedControllerPattern;

/*--------------------------- Static pattern ----------------------------------------- */

static void static_pattern_start(LedColor color)
{
    hw_platform_get_led()->set(color);
}

static void static_pattern_stop()
{
}

static LedControllerPattern static_pattern = {
    .start = static_pattern_start,
    .stop = static_pattern_stop,
};

/*--------------------------- Alert pattern ----------------------------------------- */

static bool is_led_on = false;
static LedColor alert_color;

static void alert_timer_cb(void *user_data)
{
    if (is_led_on) {
        hw_platform_get_led()->turn_off();
    } else {
        hw_platform_get_led()->set(alert_color);
    }
    is_led_on = !is_led_on;
}

/**
 * @brief Get timer instance. Used for switching between led on and off during the alert pattern.
 *
 * @return EasTimer Timer instance.
 */
static EasTimer get_timer_instance()
{
    static EasTimer instance;
    static bool is_created = false;
    if (!is_created) {
        instance =
            eas_timer_create(LED_CONTROLLER_ALERT_PATTERN_TIMER_PERIOD_MS, EAS_TIMER_PERIODIC, alert_timer_cb, NULL);
        is_created = true;
    }
    return instance;
}

static void alert_pattern_start(LedColor color)
{
    hw_platform_get_led()->set(color);
    is_led_on = true;
    alert_color = color;
    eas_timer_start(get_timer_instance());
}

static void alert_pattern_stop()
{
}

static LedControllerPattern alert_pattern = {
    .start = alert_pattern_start,
    .stop = alert_pattern_stop,
};

/**
 * @brief Return LedControllerPatterm corresponding to LedPattern.
 *
 * @param[in] pattern Led pattern.
 *
 * @return LedControllerPattern* Corresponding LedControllerPattern. NULL if LedPattern is invalid.
 */
static LedControllerPattern *led_pattern_to_controller_pattern(LedPattern pattern)
{
    LedControllerPattern *controller_pattern = NULL;
    switch (pattern) {
    case LED_PATTERN_STATIC:
        controller_pattern = &static_pattern;
        break;
    case LED_PATTERN_ALERT:
        controller_pattern = &alert_pattern;
        break;
    default:
        controller_pattern = NULL;
        break;
    }
    return controller_pattern;
}

void led_controller_turn_off()
{
    hw_platform_get_led()->turn_off();
}

void led_controller_set_color_pattern(LedColor color, LedPattern pattern)
{
    LedControllerPattern *controller_pattern = led_pattern_to_controller_pattern(pattern);
    EAS_ASSERT(controller_pattern);
    controller_pattern->start(color);
}
