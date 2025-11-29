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
 * This module is designed specifically to be used by alert_adder and alert_remover modules. alert_adder should validate
 * the alert before attempting to add it to the system. alert_remover should validate the alert id of the alert to be
 * removed before attempting to remove it from the system.
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

/**
 * @brief Check whether alert id is valid.
 *
 * @param alert_id Alert id.
 *
 * @return true Alert id is valid.
 * @return false Alert id is invalid.
 */
bool alert_validator_is_alert_id_valid(uint8_t alert_id);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_GLUE_ALERT_VALIDATOR_H */
