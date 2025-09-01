#include "stdbool.h"

#include "current_temperature.h"
#include "temperature_value.h"

static TemperatureValue get_instance()
{
    static TemperatureValue instance;
    static bool is_created = false;
    if (!is_created) {
        instance = temperature_value_create();
        is_created = true;
    }
    return instance;
}

void current_temperature_set(Temperature temperature)
{
    temperature_value_set(get_instance(), temperature);
}

Temperature current_temperature_get()
{
    return temperature_value_get(get_instance());
}

bool current_temperature_is_changed()
{
    return temperature_value_is_value_changed(get_instance());
}
