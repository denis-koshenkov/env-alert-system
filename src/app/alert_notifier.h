#ifndef ENV_ALERT_SYSTEM_SRC_APP_ALERT_NOTIFIER_H
#define ENV_ALERT_SYSTEM_SRC_APP_ALERT_NOTIFIER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Executes all necessary notifications that an alert is raised or silenced.
 *
 * This function is a stable abstract interface to be used by the AlertRaiser module.
 *
 * AlertRaiser is responsible for deciding when an alert should be raised or silenced. In either of these cases,
 * AlertRaiser calls this function and expects that all necessary notifications that the alert is now raised or silenced
 * will be executed. It is the responsibility of the implementation of this interface to ensure that.
 *
 * @param alert_id Alert id of the alert to be raised or silenced.
 * @param is_raised If true, execute notifications that the alert is raised. Otherwise, execute notifications that the
 * alert is silenced.
 */
void alert_notifier_notify(uint8_t alert_id, bool is_raised);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_ALERT_NOTIFIER_H */
