#include "temperature_value.h"

#include "eas_assert.h"

void temperature_value_set(struct temperature_value *temperature_value, temperature value)
{
    EAS_ASSERT(temperature_value);
    temperature_value->value = value;
}

temperature temperature_value_get(struct temperature_value *temperature_value)
{
    EAS_ASSERT(temperature_value);
    return temperature_value->value;
}
