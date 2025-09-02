#include "connectivity_notifier.h"
#include "config.h"

#ifndef CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS
#define CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS 1
#endif

void connectivity_notifier_notify(uint8_t alert_id, bool is_raised)
{
}
