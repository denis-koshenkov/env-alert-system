#ifndef ENV_ALERT_SYSTEM_SRC_APP_CONNECTIVITY_NOTIFIER_H
#define ENV_ALERT_SYSTEM_SRC_APP_CONNECTIVITY_NOTIFIER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Sends connectivity notifications for alerts that have connectivity notifications enabled.
 *
 * The purpose of this module is to know for which alerts the connectivity notifications are enabled, and send
 * connectivity notifications only for those alerts.
 *
 * @ref connectivity_notifier_notify should be called whenever any alert is raised or silenced. This module knows for
 * which alerts the connectivity notifications are enabled, and for which are not. It will only send notifications for
 * the alerts that have connectivity notifications enabled.
 *
 * A connectivity notification can be enabled for an alert by calling @ref connectivity_notifier_register_notification
 * with the alert id of the alert. Similarly, a connectivity notification can be disabled for an alert by calling @ref
 * connectivity_notifier_unregister_notification with the alert id of the alert.
 */

/**
 * @brief Register a connectivity notification for an alert.
 *
 * After a call to this function, all subsequent calls to @ref connectivity_notifier_notify with alert_id equal to @p
 * alert_id will result in a connectivity notification being sent.
 *
 * @param alert_id Alert id.
 */
void connectivity_notifier_register_notification(uint8_t alert_id);

/**
 * @brief Unregister a connectivity notification for an alert.
 *
 * After a call to this function, all subsequent calls to @ref connectivity_notifier_notify with alert_id equal to @p
 * alert_id will NOT result in a connectivity notification being sent.
 *
 * @param alert_id Alert id.
 */
void connectivity_notifier_unregister_notification(uint8_t alert_id);

/**
 * @brief Send a connectivity notification for an alert if the notification is registered.
 *
 * This function should be called whenever the alert is raised or silenced. This function will make sure that a
 * connectivity notification about the alert being raised/silenced is sent, if a connectivity notification is registered
 * for the alert. If a connectivity notification is not registered for the alert, nothing is done.
 *
 * @param alert_id Alert id.
 * @param is_raised If true, sends a notification that the alert is raised. Otherwise, sends a notification that the
 * alert is silenced. This only applies if the connectivity notification is currently registered for this alert.
 */
void connectivity_notifier_notify(uint8_t alert_id, bool is_raised);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_CONNECTIVITY_NOTIFIER_H */
