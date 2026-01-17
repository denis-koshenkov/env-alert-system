#ifndef ENV_ALERT_SYSTEM_SRC_HAL_DEF_LED_H
#define ENV_ALERT_SYSTEM_SRC_HAL_DEF_LED_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "led_defs.h"

/**
 * @brief Abstract HAL interface for setting the LED.
 *
 * This defines the abstract interface. The port layer should provide an implementation of this interface.
 */
typedef struct Led {
    /**
     * @brief Set LED to a ceratin color and pattern.
     *
     * After a call to this function, the LED should be displaying the requested color and pattern.
     *
     * @param led_color Color.
     * @param led_pattern Pattern.
     */
    void (*set)(LedColor led_color, LedPattern led_pattern);

    /**
     * @brief Turn off the LED.
     *
     * After a call to this function, the LED should be off.
     */
    void (*turn_off)();
} Led;

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_HAL_DEF_LED_H */
