#ifndef TEMPERATURE_VALUE_H
#define TEMPERATURE_VALUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "hal/temperature.h"

typedef struct temperature_value_struct *temperature_value;

temperature_value temperature_value_create();

#ifdef __cplusplus
}
#endif

#endif
