#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_FAKE_EAS_CURRENT_TIME_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_FAKE_EAS_CURRENT_TIME_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "eas_current_time.h"

/**
 * @brief Defines an extension to the eas_current_time interface for the unit test port.
 *
 * This extension interface defines a function to set current time, so that unit tests can control the time. Since this
 * function should not be a part of the port interface defined by the app, it is defined here in this extension header.
 * Unit tests should include this header instead of the eas_current_time.h one.
 */

void fake_eas_current_time_set(EasTime time);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_FAKE_EAS_CURRENT_TIME_H */
