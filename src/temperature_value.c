# include "temperature_value.h"

static temperature temperature_value;

void temperature_value_set(temperature temperature) {
    temperature_value = temperature;
}

temperature temperature_value_get() {
    return temperature_value;
}
