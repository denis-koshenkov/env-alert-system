#ifndef ENV_ALERT_SYSTEM_SRC_APP_HAL_LED_H
#define ENV_ALERT_SYSTEM_SRC_APP_HAL_LED_H

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

void led_set(LedColor led_color, LedPattern led_pattern);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_HAL_LED_H */
