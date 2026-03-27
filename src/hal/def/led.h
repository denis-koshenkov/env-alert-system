#ifndef ENV_ALERT_SYSTEM_SRC_HAL_DEF_LED_H
#define ENV_ALERT_SYSTEM_SRC_HAL_DEF_LED_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "led_defs.h"

/**
 * @brief Abstract HAL interface for setting the LED.
 */
typedef struct Led {
    /**
     * @brief Set LED to a ceratin color.
     *
     * After a call to this function, the LED should be displaying the requested color.
     *
     * @param led_color Color.
     */
    void (*set)(LedColor led_color);

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
