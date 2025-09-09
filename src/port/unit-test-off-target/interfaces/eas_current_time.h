#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_EAS_CURRENT_TIME_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_EAS_CURRENT_TIME_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "utils/eas_time_defs.h"

EasTime eas_current_time_get();

void fake_eas_current_time_set(EasTime time);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_EAS_CURRENT_TIME_H */
