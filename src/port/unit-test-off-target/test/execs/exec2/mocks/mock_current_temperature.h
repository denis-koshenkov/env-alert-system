#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_APP_EXEC2_MOCKS_MOCK_CURRENT_TEMPERATURE_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_APP_EXEC2_MOCKS_MOCK_CURRENT_TEMPERATURE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "temperature.h"

void current_temperature_set(Temperature temperature);

Temperature current_temperature_get();

bool current_temperature_is_changed();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_APP_EXEC2_MOCKS_MOCK_CURRENT_TEMPERATURE_H */
