#include "led_notification_executor.h"
#include "led_manager.h"
#include "eas_assert.h"

void led_notification_executor_execute(LedColor led_color, LedPattern led_pattern, bool should_be_displayed)
{
    if (should_be_displayed) {
        led_manager_add_notification(led_color, led_pattern);
    } else {
        EAS_ASSERT(led_manager_remove_notification(led_color, led_pattern));
    }
}
