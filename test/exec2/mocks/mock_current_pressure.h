#ifndef MOCK_CURRENT_PRESSURE_H
#define MOCK_CURRENT_PRESSURE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "hal/pressure.h"

void current_pressure_set(Pressure pressure);

Pressure current_pressure_get();

bool current_pressure_is_changed();

#ifdef __cplusplus
}
#endif

#endif
