#include "stdbool.h"

#include "current_humidity.h"
#include "humidity_value.h"

static HumidityValue get_instance()
{
    static HumidityValue instance;
    static bool is_created = false;
    if (!is_created) {
        instance = humidity_value_create();
        is_created = true;
    }
    return instance;
}

void current_humidity_set(Humidity humidity)
{
    humidity_value_set(get_instance(), humidity);
}

Humidity current_humidity_get()
{
    return humidity_value_get(get_instance());
}

bool current_humidity_is_changed()
{
    return humidity_value_is_value_changed(get_instance());
}
