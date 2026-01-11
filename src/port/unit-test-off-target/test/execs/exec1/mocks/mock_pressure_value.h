#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC1_MOCKS_MOCK_PRESSURE_VALUE_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC1_MOCKS_MOCK_PRESSURE_VALUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "pressure.h"

typedef struct PressureValueStruct *PressureValue;

PressureValue pressure_value_create();

void pressure_value_set(PressureValue pv, Pressure pressure);

Pressure pressure_value_get(PressureValue pv);

bool pressure_value_is_value_changed(PressureValue pv);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC1_MOCKS_MOCK_PRESSURE_VALUE_H */
