#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC1_MOCKS_MOCK_HUMIDITY_VALUE_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC1_MOCKS_MOCK_HUMIDITY_VALUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "humidity.h"

typedef struct HumidityValueStruct *HumidityValue;

HumidityValue humidity_value_create();

void humidity_value_set(HumidityValue hv, Humidity humidity);

Humidity humidity_value_get(HumidityValue hv);

bool humidity_value_is_value_changed(HumidityValue hv);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC1_MOCKS_MOCK_HUMIDITY_VALUE_H */
