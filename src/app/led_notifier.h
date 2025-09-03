#ifndef ENV_ALERT_SYSTEM_SRC_APP_LED_NOTIFIER_H
#define ENV_ALERT_SYSTEM_SRC_APP_LED_NOTIFIER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

#include "hal/led_defs.h"

/**
 * @brief Executes led notifications for alerts that have led notifications enabled.
 *
 * The purpose of this module is to know for which alerts the led notifications are enabled, and execute led
 * notifications only for those alerts.
 *
 * @ref led_notifier_notify should be called whenever any alert is raised or silenced. This module knows for
 * which alerts the led notifications are enabled, and for which are not. It will only execute notifications for the
 * alerts that have led notifications enabled.
 *
 * Led notifications can be enabled for an alert by calling @ref led_notifier_enable_notifications with the alert id of
 * the alert. Similarly, led notifications can be disabled for an alert by calling @ref
 * led_notifier_disable_notifications with the alert id of the alert.
 */

/**
 * @brief Enable led notifications for an alert.
 *
 * After a call to this function, all subsequent calls to @ref led_notifier_notify with alert_id equal to @p alert_id
 * will result in a led notification being executed with color @p led_color and pattern @p led_pattern.
 *
 * @param alert_id Alert id.
 * @param led_color Color to use for the led notification.
 * @param led_pattern Pattern to use for the led notification.
 */
void led_notifier_enable_notifications(uint8_t alert_id, LedColor led_color, LedPattern led_pattern);

/**
 * @brief Disable led notifications for an alert.
 *
 * After a call to this function, all subsequent calls to @ref led_notifier_notify with alert_id equal to @p alert_id
 * will NOT result in a led notification being executed.
 *
 * @param alert_id Alert id.
 */
void led_notifier_disable_notifications(uint8_t alert_id);

/**
 * @brief Execute a led notification for an alert if the notifications are enabled.
 *
 * This function should be called whenever the alert is raised or silenced. If the alert is raised, the led notification
 * starts being displayed with color and pattern previously passed to @ref led_notifier_enable_notifications. If the
 * alert is silenced, the led notification with those color and pattern will stop being displayed.
 *
 * Note that the above applies only if led notifications are enabled for the alert. If led notifications are disabled
 * for the alert, this function does nothing.
 *
 * @param alert_id Alert id.
 * @param is_raised If true, starts displaying the led notification for this alert. Otherwise, stops displaying the led
 * notification for this alert. This only applies if led notifications are currently enabled for this alert.
 */
void led_notifier_notify(uint8_t alert_id, bool is_raised);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_LED_NOTIFIER_H */
