#include "alert_notifier.h"
#include "connectivity_notifier.h"
#include "led_notifier.h"

void alert_notifier_notify(uint8_t alert_id, bool is_raised)
{
    connectivity_notifier_notify(alert_id, is_raised);
    led_notifier_notify(alert_id, is_raised);
}
