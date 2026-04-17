#ifndef ENV_ALERT_SYSTEM_SRC_APP_LED_CONTROLLER_H
#define ENV_ALERT_SYSTEM_SRC_APP_LED_CONTROLLER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "led_defs.h"

/**
 * @brief Implements led patterns.
 *
 * This module sits between application modules that want to set led to a certain color and pattern, and HAL module that
 * sets the led to a certain color.
 *
 * This module manipulates the led in a way to implement different led patterns.
 */

/**
 * @brief Turn off the led.
 *
 * Stops any ongoing color patterns.
 */
void led_controller_turn_off();

/**
 * @brief Set led to a certain color with a certain pattern.
 *
 * It is allowed to call this function if the led is already set to this color and pattern.
 *
 * @param[in] color Color.
 * @param[in] pattern Pattern.
 */
void led_controller_set_color_pattern(LedColor color, LedPattern pattern);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_LED_CONTROLLER_H */
