#ifndef ENV_ALERT_SYSTEM_SRC_APP_GLUE_ALERT_REMOVER_H
#define ENV_ALERT_SYSTEM_SRC_APP_GLUE_ALERT_REMOVER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/**
 * @brief Remove an alert from the system.
 *
 * This function should be called whenever a "remove alert" message is received via the connection interface.
 * Does nothing if the alert id is invalid, or if an alert with the specified alert id does not exist in the system.
 *
 * @param alert_id Alert id of the alert to be removed.
 * @param user_data User data. Unused, added to the function signature so that this function can be registered as a
 * "remove alert" callback with the msg_transceiver module.
 */
void alert_remover_remove_alert(uint8_t alert_id, void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_GLUE_ALERT_REMOVER_H */
