#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_APP_EXEC2_MOCKS_MOCK_CURRENT_PRESSURE_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_APP_EXEC2_MOCKS_MOCK_CURRENT_PRESSURE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "pressure.h"

void current_pressure_set(Pressure pressure);

Pressure current_pressure_get();

bool current_pressure_is_changed();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_APP_EXEC2_MOCKS_MOCK_CURRENT_PRESSURE_H */
