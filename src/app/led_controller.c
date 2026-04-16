#include "led_controller.h"
#include "hw_platform.h"
#include "eas_timer.h"
#include "eas_assert.h"

#define LED_CONTROLLER_ALERT_PATTERN_TIMER_PERIOD_MS 300

typedef struct {
    /**
     * @brief Start a pattern.
     *
     * @pre No pattern is currently ongoing, the led is off.
     *
     * Call this function to start a pattern if this pattern is not already active.
     *
     * @param[in] color Led color.
     */
    void (*start)(LedColor color);

    /**
     * @brief Set color for an already ongoing pattern.
     *
     * Call this function only if this pattern is already ongoing, but with a different color. This function must not be
     * called when:
     * - This pattern is not ongoing; or
     * - This pattern is already ongoing with @p color.
     *
     * @param[in] color Color to set.
     */
    void (*set_color)(LedColor color);

    /**
     * @brief Stop a currently ongoing pattern.
     *
     * The caller must check that the pattern is already ongoing before calling this function.
     */
    void (*stop)();
} LedControllerPattern;

/** Color that is being displayed. Only valid if is_active == true. */
static LedColor current_color;
/** Pattern that is being displayed. Only valid if is_active == true. */
static LedPattern current_pattern;
/** True if a color pattern is being displayed, false otherwise. */
static bool is_active = false;

/*--------------------------- Static pattern ----------------------------------------- */

static void static_pattern_start(LedColor color)
{
    hw_platform_get_led()->set(color);
}

static void static_pattern_set_color(LedColor color)
{
}

static void static_pattern_stop()
{
}

static LedControllerPattern static_pattern = {
    .start = static_pattern_start,
    .set_color = static_pattern_set_color,
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

/**
 * @brief Check if an alert pattern is currently active.
 *
 * @retval true Alert pattern is currently active.
 * @retval false Another pattern is active or no pattern is active.
 */
static bool is_alert_pattern_active()
{
    return (is_active && (current_pattern == LED_PATTERN_ALERT));
}

static void alert_pattern_start(LedColor color)
{
    hw_platform_get_led()->set(color);
    is_led_on = true;
    alert_color = color;
    eas_timer_start(get_timer_instance());
}

static void alert_pattern_set_color(LedColor color)
{
    if (is_led_on) {
        hw_platform_get_led()->set(color);
    }
    /* If the led is off, we only need to assign the color to a variable, so that when the timer next expires, it will
     * set the new color. */
    alert_color = color;
}

static void alert_pattern_stop()
{
}

static LedControllerPattern alert_pattern = {
    .start = alert_pattern_start,
    .set_color = alert_pattern_set_color,
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

/**
 * @brief Initialize local state to indicate that a color pattern is being displayed.
 *
 * @param[in] color Color to set.
 * @param[in] pattern Pattern to set.
 */
static void set_current_color_pattern(LedColor color, LedPattern pattern)
{
    current_color = color;
    current_pattern = pattern;
    is_active = true;
}

/**
 * @brief Set local state to indicate that no color pattern is currently being displayed.
 */
static void clear_current_color_pattern()
{
    is_active = false;
}

void led_controller_turn_off()
{
    clear_current_color_pattern();
    hw_platform_get_led()->turn_off();
}

void led_controller_set_color_pattern(LedColor color, LedPattern pattern)
{
    if (is_active && (color == current_color) && (pattern == current_pattern)) {
        /* These color and pattern are already set */
        return;
    }

    LedControllerPattern *controller_pattern = led_pattern_to_controller_pattern(pattern);
    EAS_ASSERT(controller_pattern);
    EAS_ASSERT(controller_pattern->start);
    EAS_ASSERT(controller_pattern->set_color);

    if (is_active && (pattern == current_pattern)) {
        /* This pattern is already ongoing, but with a different color */
        controller_pattern->set_color(color);
    } else {
        /* This pattern is not ongoing */
        controller_pattern->start(color);
    }

    /* Must be done after calling start, because the start logic needs to check the current state before the call to
     * this function */
    set_current_color_pattern(color, pattern);
}
