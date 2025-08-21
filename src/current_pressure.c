#include "stdbool.h"

#include "current_pressure.h"
#include "pressure_value.h"

static PressureValue get_instance()
{
    static PressureValue instance;
    static bool is_created = false;
    if (!is_created) {
        instance = pressure_value_create();
        is_created = true;
    }
    return instance;
}

void current_pressure_set(Pressure pressure)
{
    pressure_value_set(get_instance(), pressure);
}

Pressure current_pressure_get()
{
    return pressure_value_get(get_instance());
}

bool current_pressure_is_changed()
{
    return pressure_value_is_value_changed(get_instance());
}
