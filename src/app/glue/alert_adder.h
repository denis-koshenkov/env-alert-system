#ifndef ENV_ALERT_SYSTEM_SRC_APP_GLUE_ALERT_ADDER_H
#define ENV_ALERT_SYSTEM_SRC_APP_GLUE_ALERT_ADDER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "msg_transceiver.h"

/**
 * @brief Add an alert to the system.
 *
 * This function should be called whenever a "add alert" message is received via the connection interface.
 * Does nothing if the alert is invalid, or if an alert with the specified alert id already exists in the system.
 *
 * @param alert Alert to add.
 */
void alert_adder_add_alert(const MsgTransceiverAlert *const alert);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_GLUE_ALERT_ADDER_H */
