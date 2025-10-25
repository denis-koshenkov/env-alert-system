#ifndef ENV_ALERT_SYSTEM_SRC_APP_ALERT_RAISER_H
#define ENV_ALERT_SYSTEM_SRC_APP_ALERT_RAISER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

#include "alert_raiser_defs.h"

/**
 * @brief Decides when an alert is raised/silenced based on alert condition result, warmup period, and cooldown period.
 *
 * # Raising and Silencing of Alerts
 *
 * In general, an alert is raised when its alert condition is true, and it is silenced when the alert condition is
 * false. The only exceptions to this rule are when an alert is in its warmup period or cooldown period.
 *
 * The warmup period is the amount of time for which the alert condition needs to evaluate to true before the alert is
 * raised. For example, the warmup period is 10 seconds. The alert condition becomes true. After 5 seconds, the alert
 * condition becomes false again. In this case, the alert is not raised, because the alert condition has not been true
 * for 10 seconds - the duration of the warmup period. However, if the alert condition becomes true and stays true for
 * at least 10 seconds, the alert will be raised. The alert will be raised 10 seconds after the alert condition changes
 * to true.
 *
 * The warmup period is allowed to be 0. In that case, the alert is raised immediately, as soon as the alert condition
 * becomes true.
 *
 * The cooldown period is similar to the warmup period, but for silencing the alarm. The cooldown period is the amount
 * of time the alert condition needs to evaluate to false before the alert is silenced. For example, the alert is
 * currently raised, and the alert condition is true. When the alert condition becomes false, the alert is not silenced
 * immediately. It is only silenced if the alert condition stays false for at least the duration of the cooldown period.
 * If the alert condition changes back to true before the cooldown period expired, the alert is not silenced - it stays
 * raised.
 *
 * The cooldown period is allowed to be 0. In that case, the alert is silenced immediately, as soon as the alert
 * condition changes from true to false.
 *
 * # Alert Raiser
 *
 * One alert raiser instance decides whether one alert is raised/silenced based on its alert condition result, warmup
 * period, and cooldown period. The alert raiser should always be notified as soon as the condition result of the alert
 * changes by calling @ref alert_raiser_set_alert_condition_result.
 *
 * It is possible to reuse an alert raiser instance for another alert. @ref alert_raiser_set_alert can be called at any
 * time to set a new alert for the alert raiser instance. Note that if @ref alert_raiser_set_alert is called when the
 * current alert for this alert raiser is raised, the alert raiser first silences the current alert, and only then
 * updates its internal state to represent the new alert.
 *
 * Whenever an alert is set by calling @ref alert_raiser_set_alert, it is initially considered to be
 * silenced, and its alert condition is considered to be false. If the alert condition is true at the moment when @ref
 * alert_raiser_set_alert is called, call @ref alert_raiser_set_alert_condition_result with alert_condition_result
 * parameter set to true immediately after calling @ref alert_raiser_set_alert.
 *
 * # Usage
 * ```
 * // Create an alert raiser instance
 * AlertRaiser alert_raiser = alert_raiser_create();
 *
 * // Set an alert with id 1, warmup period 1000 ms, cooldown period 2000 ms
 * alert_raiser_set_alert(alert_raiser, 1, 1000, 2000);
 *
 * // Now, keep notifying alert raiser whenever the alert condition result for alert with id 1 changes
 * // This way, the alert raiser can correctly decide when to raise/silence the alarm
 * alert_raiser_set_alert_condition_result(alert_raiser, true);
 * alert_raiser_set_alert_condition_result(alert_raiser, false);
 * ```
 *
 * # Notifications for Raising and Silencing Alerts
 * Whenever an alert should be raised/silenced, alert raiser calls a module that is reponsible for executing all
 * registered notifications for raising/silencing the alert.
 *
 * Thus, the caller of this module only needs to keep updating the alert condition result, and the alert raiser will
 * ensure that all notifications for raising/silencing the alert are executed.
 */

/**
 * @brief Create an alert raiser instance.
 *
 * @return AlertRaiser Created alert raiser instance.
 */
AlertRaiser alert_raiser_create();

/**
 * @brief Set alert.
 *
 * If an alert was previously set for this alert raiser instance, and it is in the raised state when this function is
 * called to set a new alert, the implementation first silences the old alert, and then sets the new alert.
 *
 * @param self Alert raiser instance created by @ref alert_raiser_create.
 * @param alert_id Alert id of the alert.
 * @param warmup_period_ms Warmup period of the alert in milliseconds.
 * @param cooldown_period_ms Cooldown period of the alert in milliseconds.
 */
void alert_raiser_set_alert(AlertRaiser self, uint8_t alert_id, uint32_t warmup_period_ms, uint32_t cooldown_period_ms);

void alert_raiser_unset_alert(AlertRaiser self);

bool alert_raiser_is_alert_set(AlertRaiser self);

/**
 * @brief Set alert condition result for the alert.
 *
 * @param self Alert raiser instance created by @ref alert_raiser_create.
 * @param alert_condition_result The alert condition result.
 *
 * @note Fires an assert if @ref alert_raiser_set_alert has not yet been called for this alert raiser instance. In that
 * case, the alert raiser does not know the id, warmup period, and cooldown period of the alert to raise/silence, so it
 * cannot raise/silence any alert. This function should only be called after an alert is set by calling @ref
 * alert_raiser_set_alert.
 */
void alert_raiser_set_alert_condition_result(AlertRaiser self, bool alert_condition_result);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_ALERT_RAISER_H */
