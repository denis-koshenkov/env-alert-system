#include "led_notification_executor.h"

void led_notification_executor_execute(LedColor led_color, LedPattern led_pattern, bool should_be_displayed)
{
    /* Empty for now, because unit tests mock this interface instead of using the real one (this one). The
     * implementation here should call LedManager functions add_notification and remove_notification, based on whether
     * the notification should be displayed or not. */
}
