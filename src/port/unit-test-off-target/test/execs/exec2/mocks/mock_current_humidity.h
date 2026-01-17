#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC2_MOCKS_MOCK_CURRENT_HUMIDITY_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC2_MOCKS_MOCK_CURRENT_HUMIDITY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "hal/humidity.h"

void current_humidity_set(Humidity humidity);

Humidity current_humidity_get();

bool current_humidity_is_changed();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC2_MOCKS_MOCK_CURRENT_HUMIDITY_H */
