#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_APP_EXEC1_MOCKS_MOCK_TEMPERATURE_VALUE_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_APP_EXEC1_MOCKS_MOCK_TEMPERATURE_VALUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "temperature.h"

typedef struct TemperatureValueStruct *TemperatureValue;

TemperatureValue temperature_value_create();

void temperature_value_set(TemperatureValue tv, Temperature temperature);

Temperature temperature_value_get(TemperatureValue tv);

bool temperature_value_is_value_changed(TemperatureValue tv);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_APP_EXEC1_MOCKS_MOCK_TEMPERATURE_VALUE_H */
