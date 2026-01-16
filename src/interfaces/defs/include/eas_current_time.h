#ifndef ENV_ALERT_SYSTEM_SRC_INTERFACES_DEFS_INCLUDE_EAS_CURRENT_TIME_H
#define ENV_ALERT_SYSTEM_SRC_INTERFACES_DEFS_INCLUDE_EAS_CURRENT_TIME_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "utils/eas_time_defs.h"

/**
 * @brief Get current time in ms.
 *
 * @return EasTime Time in ms since the device was booted.
 */
EasTime eas_current_time_get();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_INTERFACES_DEFS_INCLUDE_EAS_CURRENT_TIME_H */
