#ifndef ENV_ALERT_SYSTEM_SRC_APP_LED_SETTER_H
#define ENV_ALERT_SYSTEM_SRC_APP_LED_SETTER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "led_defs.h"

/**
 * @brief Set LED to the specified color and pattern.
 *
 * This function is a stable abstract interface to be used by the LedManager module.
 *
 * LedManager calls this function whenever it needs to set the led to a certain color and pattern. This abstract
 * interface guarantees that the led gets set to these color and pattern.
 *
 * @param color Led color.
 * @param pattern Led pattern.
 */
void led_setter_set(LedColor led_color, LedPattern led_pattern);

/**
 * @brief Turn off LED.
 *
 * This function is a stable abstract interface for LedManager to turn off the LED.
 */
void led_setter_turn_off();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_LED_SETTER_H */
