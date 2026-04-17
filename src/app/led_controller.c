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
     *
     * The implementation must NOT turn off the led. It should only clean up all the internal state of the pattern (e.g.
     * stopping alert pattern timer). Generic logic turns off the led when when a pattern is stopped.
     */
    void (*stop)();
} LedControllerPattern;

/** Color that is being displayed. Only valid if current_controller_pattern != NULL. */
static LedColor current_color;
/** Pattern that is being displayed. Only valid if current_controller_pattern != NULL. */
static LedPattern current_pattern;
/** Pointer to pattern object that corresponds to pattern being displayed. NULL if no pattern is being displayed. */
static const LedControllerPattern *current_controller_pattern = NULL;

/**
 * @brief Check if a pattern is currently being displayed.
 *
 * @retval true A pattern is being displayed.
 * @retval false No pattern is being displayed.
 */
static bool is_displaying_pattern()
{
    return (current_controller_pattern != NULL);
}

/*--------------------------- Static pattern ----------------------------------------- */

static void static_pattern_start(LedColor color)
{
    hw_platform_get_led()->set(color);
}

static void static_pattern_set_color(LedColor color)
{
    hw_platform_get_led()->set(color);
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

/**
 * @brief Check if an alert pattern is currently being displayed.
 *
 * @retval true Alert pattern is currently being displayed.
 * @retval false Another pattern is active or no pattern is being displayed.
 */
static bool is_displaying_alert_pattern()
{
    return (is_displaying_pattern() && (current_pattern == LED_PATTERN_ALERT));
}

/**
 * @brief Alert timer callback.
 *
 * Toggles between led on and off for the alert pattern. Does nothing if alert pattern is not ongoing anymore.
 *
 * @param[in] user_data Unused.
 */
static void alert_timer_cb(void *user_data)
{
    if (!is_displaying_alert_pattern()) {
        return;
    }

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
    eas_timer_stop(get_timer_instance());
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
static void set_current_color_pattern(LedColor color, LedPattern pattern,
                                      const LedControllerPattern *controller_pattern)
{
    EAS_ASSERT(controller_pattern);
    current_color = color;
    current_pattern = pattern;
    current_controller_pattern = controller_pattern;
}

/**
 * @brief Set local state to indicate that no color pattern is currently being displayed.
 */
static void clear_current_color_pattern()
{
    current_controller_pattern = NULL;
}

void led_controller_turn_off()
{
    if (is_displaying_pattern()) {
        /* current_controller_pattern is not NULL if and only if a pattern is being displayed */
        EAS_ASSERT(current_controller_pattern);
        current_controller_pattern->stop();
        clear_current_color_pattern();
        hw_platform_get_led()->turn_off();
    }
}

void led_controller_set_color_pattern(LedColor color, LedPattern pattern)
{
    if (is_displaying_pattern() && (color == current_color) && (pattern == current_pattern)) {
        /* These color and pattern are already set */
        return;
    }

    LedControllerPattern *controller_pattern = led_pattern_to_controller_pattern(pattern);
    EAS_ASSERT(controller_pattern);
    EAS_ASSERT(controller_pattern->start);
    EAS_ASSERT(controller_pattern->set_color);
    EAS_ASSERT(controller_pattern->stop);

    if (is_displaying_pattern() && (pattern == current_pattern)) {
        /* This pattern is already ongoing, but with a different color */
        controller_pattern->set_color(color);
    } else {
        if (is_displaying_pattern()) {
            /* A different pattern is ongoing - needs to be stopped before starting this one */
            current_controller_pattern->stop(color);
        }
        controller_pattern->start(color);
    }

    set_current_color_pattern(color, pattern, controller_pattern);
}
