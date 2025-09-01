#ifndef MOCK_PRESSURE_VALUE_H
#define MOCK_PRESSURE_VALUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "hal/pressure.h"

typedef struct PressureValueStruct *PressureValue;

PressureValue pressure_value_create();

void pressure_value_set(PressureValue pv, Pressure pressure);

Pressure pressure_value_get(PressureValue pv);

bool pressure_value_is_value_changed(PressureValue pv);

#ifdef __cplusplus
}
#endif

#endif
