#ifndef MOCK_HUMIDITY_VALUE_H
#define MOCK_HUMIDITY_VALUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "hal/humidity.h"

typedef struct HumidityValueStruct *HumidityValue;

HumidityValue humidity_value_create();

void humidity_value_set(HumidityValue hv, Humidity humidity);

Humidity humidity_value_get(HumidityValue hv);

bool humidity_value_is_value_changed(HumidityValue hv);

#ifdef __cplusplus
}
#endif

#endif
