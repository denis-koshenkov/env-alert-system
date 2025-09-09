#ifndef ENV_ALERT_SYSTEM_SRC_APP_UTILS_EAS_TIME_H
#define ENV_ALERT_SYSTEM_SRC_APP_UTILS_EAS_TIME_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "utils/eas_time_defs.h"

/**
 * @brief Offset a time point into the future.
 *
 * @param time Time obtained by calling @ref eas_current_time_get.
 * @param offset_ms By how many ms the time should be offset into the future.
 *
 * @return EasTime A time point that represents @p time offset into the future by @p offset.
 */
EasTime eas_time_offset_into_future(EasTime time, size_t offset_ms);

/**
 * @brief Check if time1 is equal to time2 or represents time after time2.
 *
 * The implementation assumes that time1 and time2 have been obtained within 2147483647 ms within each other. This
 * corresponds to ~24.85 24-hour days. If time1 and time2 were obtained with a gap of more than 2147483647 ms within
 * each other, the result of this function is undefined.
 *
 * @param time1 Time obtained by calling @ref eas_current_time_get at one point.
 * @param time2 Time obtained by calling @ref eas_current_time_get at potentially another point.
 *
 * @return true time1 is either equal to time2, or represents time after time2.
 * @return false time1 represents time before time2.
 */
bool eas_time_is_equal_or_after(EasTime time1, EasTime time2);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_UTILS_EAS_TIME_H */
