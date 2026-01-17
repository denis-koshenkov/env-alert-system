#ifndef ENV_ALERT_SYSTEM_SRC_APP_LED_MANAGER_H
#define ENV_ALERT_SYSTEM_SRC_APP_LED_MANAGER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "hal/led_defs.h"

/**
 * @brief Displays all added led notifications by periodically switching between them.
 *
 *   - If there are no added notifications, the led is off.
 *   - If there is one added notification, the led is set to display that notification.
 *   - If there are two or more added notifications, the led manager periodically switches between all added
 * notifications. Each notification is displayed for CONFIG_LED_MANAGER_NOTIFICATION_DURATION_SECONDS seconds before
 * switching to the next notification.
 *
 * It is allowed to add several notifications with the same color and pattern. They will be treated as separate
 * notifications. Imagine that there are several notifications with the same color and pattern, and @ref
 * led_manager_remove_notification is called for these color and pattern. Only one notification with these color and
 * pattern is removed, the other notifications are not removed and will still be displayed.
 *
 * This module expects the following configs to be defined in config.h:
 *   - CONFIG_LED_MANAGER_NOTIFICATION_DURATION_SECONDS - for how long each notification is displayed.
 *   - CONFIG_LED_MANAGER_IGNORE_TIMER_MARGIN_MS - how much earlier than the timer's expected expiration time a timer
 * expiry callback function can be executed.
 */

/**
 * @brief Add a led notification to be displayed.
 *
 * @param led_color Led color for this notification.
 * @param led_pattern Led pattern for this notification.
 */
void led_manager_add_notification(LedColor led_color, LedPattern led_pattern);

/**
 * @brief Remove a led notification. It will no longer be displayed.
 *
 * @param led_color Led color of the notification to remove.
 * @param led_pattern Led pattern of the notification to remove.
 *
 * @return true Successfully found and removed a led notification with @p led_color and @p led_pattern. If there were
 * several notifications with these color and pattern, only one of them was removed.
 * @return false There are no notifications with these color and pattern. Nothing is done.
 */
bool led_manager_remove_notification(LedColor led_color, LedPattern led_pattern);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_LED_MANAGER_H */
