#ifndef MOCK_CURRENT_HUMIDITY_H
#define MOCK_CURRENT_HUMIDITY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "hal/humidity.h"

void current_humidity_set(Humidity humidity);

Humidity current_humidity_get();

bool current_humidity_is_changed();

#ifdef __cplusplus
}
#endif

#endif
