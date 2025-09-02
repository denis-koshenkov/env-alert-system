#ifndef ENV_ALERT_SYSTEM_SRC_APP_CONNECTIVITY_NOTIFICATION_SENDER_H
#define ENV_ALERT_SYSTEM_SRC_APP_CONNECTIVITY_NOTIFICATION_SENDER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Sends a connectivity notification that an alert is raised or silenced.
 *
 * This function is a stable abstract interface to be used by the ConnectivityNotifier module.
 *
 * ConnectivityNotifier calls this function whenever a notification that an alert is raised/silenced needs to be sent
 * via connectivity. This abstract interface guarantees that such a notification is sent whenever this function is
 * called.
 *
 * @param alert_id Alert id of the alert to be raised or silenced.
 * @param is_raised If true, sends a notification that the alert is raised. Otherwise, sends a notification that the
 * alert is silenced.
 */
void connectivity_notification_sender_send(uint8_t alert_id, bool is_raised);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_CONNECTIVITY_NOTIFICATION_SENDER_H */
