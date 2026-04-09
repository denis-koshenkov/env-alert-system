#ifndef ENV_ALERT_SYSTEM_SRC_APP_LED_CONTROLLER_H
#define ENV_ALERT_SYSTEM_SRC_APP_LED_CONTROLLER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "led_defs.h"

void led_controller_turn_off();

void led_controller_set_color_pattern(LedColor color, LedPattern pattern);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_LED_CONTROLLER_H */
