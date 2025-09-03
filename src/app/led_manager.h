#ifndef ENV_ALERT_SYSTEM_SRC_APP_LED_MANAGER_H
#define ENV_ALERT_SYSTEM_SRC_APP_LED_MANAGER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "hal/led_defs.h"

void led_manager_add_notification(LedColor led_color, LedPattern led_pattern);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_LED_MANAGER_H */
