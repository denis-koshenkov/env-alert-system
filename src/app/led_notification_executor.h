#ifndef ENV_ALERT_SYSTEM_SRC_APP_LED_NOTIFICATION_EXECUTOR_H
#define ENV_ALERT_SYSTEM_SRC_APP_LED_NOTIFICATION_EXECUTOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "led_defs.h"

/**
 * @brief Executes a led notification that an alert is raised or silenced.
 *
 * This function is a stable abstract interface to be used by the LedNotifier module.
 *
 * LedNotifier calls this function whenever a led notification, which is a led color and pattern, needs to start being
 * displayed or stop being displayed. This interface guarantees that the led notification will start being displayed or
 * stop being displayed as requested.
 *
 * @param led_color Color of the led notification.
 * @param led_pattern Pattern of the led notification.
 * @param should_be_displayed If true, the led notification with the specified color and pattern will start being
 * displayed. Otherwise, the led notification with the specified color and pattern will stop being displayed.
 *
 * @note This interface does not provide any details on how exactly the led notification is displayed. The LedNotifier
 * only knows about the concept of "executing" the led notification - the exact details on how exactly the notification
 * is displayed are managed by other modules.
 *
 * @warning The implementation is allowed to assume that if @p should_be_displayed is false, then previously there was a
 * call to this function with the same @p led_color and @p led_pattern, but with @p should_be_displayed as true. In
 * general, only notifications that were previously requested to be displayed, should be requested to not be displayed
 * anymore. If this is violated, this function raises an assert.
 */
void led_notification_executor_execute(LedColor led_color, LedPattern led_pattern, bool should_be_displayed);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_LED_NOTIFICATION_EXECUTOR_H */
