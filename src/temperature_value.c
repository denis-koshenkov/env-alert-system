#include "temperature_value.h"

#include "eas_assert.h"

void temperature_value_init(struct temperature_value *temperature_value)
{
    temperature_value->set_has_been_called = false;
    temperature_value->initialized = true;
    temperature_value->value_changed = false;
}

void temperature_value_set(struct temperature_value *temperature_value, temperature value)
{
    EAS_ASSERT(temperature_value);
    EAS_ASSERT(temperature_value->initialized);

    /* temperature_value->value is only initialized if this function - set() - has been called previously. */
    if (temperature_value->set_has_been_called) {
        temperature_value->value_changed = (temperature_value->value != value);
    } else {
        /* This is the first ever call to set() - is_value_changed() should return true after the first call to set().*/
        temperature_value->value_changed = true;
    }

    temperature_value->value = value;
    temperature_value->set_has_been_called = true;
}

temperature temperature_value_get(struct temperature_value *temperature_value)
{
    EAS_ASSERT(temperature_value);
    EAS_ASSERT(temperature_value->initialized);

    return temperature_value->value;
}

bool temperature_value_is_value_changed(struct temperature_value *temperature_value)
{
    EAS_ASSERT(temperature_value);
    EAS_ASSERT(temperature_value->initialized);
    EAS_ASSERT(temperature_value->set_has_been_called);

    return temperature_value->value_changed;
}

bool temperature_value_assert_test(struct temperature_value *temperature_value)
{
    EAS_ASSERT(temperature_value);
    return temperature_value->initialized;
}
