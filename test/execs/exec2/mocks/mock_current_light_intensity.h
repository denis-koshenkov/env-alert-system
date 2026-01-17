#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC2_MOCKS_MOCK_CURRENT_LIGHT_INTENSITY_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC2_MOCKS_MOCK_CURRENT_LIGHT_INTENSITY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "light_intensity.h"

void current_light_intensity_set(LightIntensity light_intensity);

LightIntensity current_light_intensity_get();

bool current_light_intensity_is_changed();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC2_MOCKS_MOCK_CURRENT_LIGHT_INTENSITY_H */
