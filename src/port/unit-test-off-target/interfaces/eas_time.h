#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_EAS_TIME_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_EAS_TIME_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint32_t EasTime;

EasTime eas_time_get();

EasTime eas_time_offset_into_future(EasTime time, size_t offset_ms);

/**
 * @brief Check if time1 is equal to time2 or represents time after time2.
 *
 * @param time1 Time obtained by calling @ref eas_time_get at one point.
 * @param time2 Time obtained by calling @ref eas_time_get at potentially another point.
 *
 * @return true time1 is either equal to time2, or represents time after time2.
 * @return false time1 represents time before time2.
 */
bool eas_time_is_equal_or_after(EasTime time1, EasTime time2);

void fake_eas_time_set_is_equal_or_after_return_value(bool return_value);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_EAS_TIME_H */
