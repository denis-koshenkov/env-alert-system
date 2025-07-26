#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <stdbool.h>

#include "hal/temperature.h"

struct temperature_value {
    temperature value;
};

void temperature_value_set(struct temperature_value *temperature_value, temperature value);

temperature temperature_value_get(struct temperature_value *temperature_value);

bool temperature_value_is_value_changed(struct temperature_value *temperature_value);

#endif
