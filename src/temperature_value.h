#ifndef TEMPERATURE_VALUE_H
#define TEMPERATURE_VALUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "hal/temperature.h"

typedef struct temperature_value_struct *temperature_value;

temperature_value temperature_value_create();

void temperature_value_set(temperature_value tv, temperature temperature);

temperature temperature_value_get(temperature_value tv);

#ifdef __cplusplus
}
#endif

#endif
