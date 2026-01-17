#ifndef ENV_ALERT_SYSTEM_SRC_HAL_DEF_LED_DEFS_H
#define ENV_ALERT_SYSTEM_SRC_HAL_DEF_LED_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

/**< Color to display with the led. */
typedef enum LedColor {
    LED_COLOR_RED,
    LED_COLOR_GREEN,
    LED_COLOR_BLUE,
} LedColor;

/**< Pattern to display with the led. */
typedef enum LedPattern {
    /**< The color is displayed without any changes in led intensity. */
    LED_PATTERN_STATIC,
    /**< The led is blinking fast to showcase urgency. */
    LED_PATTERN_ALERT,
} LedPattern;

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_HAL_DEF_LED_DEFS_H */
