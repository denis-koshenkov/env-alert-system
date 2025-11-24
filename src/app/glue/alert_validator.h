#ifndef ENV_ALERT_SYSTEM_SRC_APP_GLUE_ALERT_VALIDATOR_H
#define ENV_ALERT_SYSTEM_SRC_APP_GLUE_ALERT_VALIDATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "msg_transceiver.h"

bool alert_validator_is_alert_valid(const MsgTransceiverAlert *alert);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_GLUE_ALERT_VALIDATOR_H */
