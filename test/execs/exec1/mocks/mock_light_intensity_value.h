#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC1_MOCKS_MOCK_LIGHT_INTENSITY_VALUE_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC1_MOCKS_MOCK_LIGHT_INTENSITY_VALUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "light_intensity.h"

typedef struct LightIntensityValueStruct *LightIntensityValue;

LightIntensityValue light_intensity_value_create();

void light_intensity_value_set(LightIntensityValue liv, LightIntensity light_intensity);

LightIntensity light_intensity_value_get(LightIntensityValue liv);

bool light_intensity_value_is_value_changed(LightIntensityValue liv);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC1_MOCKS_MOCK_LIGHT_INTENSITY_VALUE_H */
