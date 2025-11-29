#ifndef ENV_ALERT_SYSTEM_SRC_APP_GLUE_ALERT_VALIDATOR_H
#define ENV_ALERT_SYSTEM_SRC_APP_GLUE_ALERT_VALIDATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "msg_transceiver.h"

/**
 * @brief Validates whether an alert is valid.
 *
 * This module is designed specifically to be used by the alert_adder module. alert_adder should validate the alert
 * before attempting to add it to the system.
 */

/**
 * @brief Check whether an alert is valid.
 *
 * Check the validity of alert ID, notification types, led color and pattern (if applicable), and alert condition.
 *
 * @param alert Alert to validate.
 *
 * @return true Alert is valid.
 * @return false Alert is invalid.
 */
bool alert_validator_is_alert_valid(const MsgTransceiverAlert *const alert);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_GLUE_ALERT_VALIDATOR_H */
