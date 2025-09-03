#ifndef ENV_ALERT_SYSTEM_SRC_APP_LED_NOTIFIER_H
#define ENV_ALERT_SYSTEM_SRC_APP_LED_NOTIFIER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

#include "hal/led_defs.h"

void led_notifier_notify(uint8_t alert_id, bool is_raised);

void led_notifier_enable_notifications(uint8_t alert_id, LedColor led_color, LedPattern led_pattern);

void led_notifier_disable_notifications(uint8_t alert_id);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_LED_NOTIFIER_H */
