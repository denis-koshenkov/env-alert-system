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
 */
void alert_remover_remove_alert(uint8_t alert_id);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_GLUE_ALERT_REMOVER_H */
