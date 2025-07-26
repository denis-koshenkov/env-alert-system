#include "e_assert.h"

#include "temperature_value.h"

void temperature_value_set(struct temperature_value *temperature_value, temperature value) {
    temperature_value->value = value;
}

temperature temperature_value_get(struct temperature_value *temperature_value) {
    return temperature_value->value;
}

bool temperature_value_is_value_changed(struct temperature_value *temperature_value) {
    E_ASSERT(true);
    return true;
}
