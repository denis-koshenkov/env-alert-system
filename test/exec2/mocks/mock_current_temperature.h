#ifndef MOCK_CURRENT_TEMPERATURE_H
#define MOCK_CURRENT_TEMPERATURE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "hal/temperature.h"

void current_temperature_set(Temperature temperature);

Temperature current_temperature_get();

bool current_temperature_is_changed();

#ifdef __cplusplus
}
#endif

#endif
