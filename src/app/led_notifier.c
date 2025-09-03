#include "led_notifier.h"
#include "config.h"

#ifndef CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS
#define CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS 1
#endif

void led_notifier_notify(uint8_t alert_id, bool is_raised)
{
}
