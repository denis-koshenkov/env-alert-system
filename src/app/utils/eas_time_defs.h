#ifndef ENV_ALERT_SYSTEM_SRC_APP_UTILS_EAS_TIME_DEFS_H
#define ENV_ALERT_SYSTEM_SRC_APP_UTILS_EAS_TIME_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/** Defined in a separate header, so that it can be used by both eas_time module and eas_current_time. eas_time is
 * defined as a part of the application, and implements time operations such as offsetting into future, and checking if
 * one time point is before/after another time point. eas_current_time is implemented by the port. eas_current_time has
 * a getter that gets current time. Both of these modules need access to the EasTime type definition. */

typedef uint32_t EasTime;

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_UTILS_EAS_TIME_DEFS_H */
