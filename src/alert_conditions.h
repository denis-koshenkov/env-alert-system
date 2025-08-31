#ifndef ALERT_CONDITIONS_H
#define ALERT_CONDITIONS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "alert_condition_defs.h"

void alert_conditions_set_alert_condition(uint8_t alert_id, AlertCondition alert_condition);

AlertCondition alert_conditions_get_alert_condition(uint8_t alert_id);

#ifdef __cplusplus
}
#endif

#endif
