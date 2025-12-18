#include <stddef.h>

#include "hw_platform.h"
#include "virtual_led_mock.h"

/* Unused in unit test port */
void hw_platform_init()
{
}

const Led *const hw_platform_get_led()
{
    return virtual_led_mock_get();
}

/* Unused in unit test port */
const TemperatureSensor *const hw_platform_get_temperature_sensor()
{
    return NULL;
}

/* Unused in unit test port */
const PressureSensor *const hw_platform_get_pressure_sensor()
{
    return NULL;
}

/* Unused in unit test port */
const HumiditySensor *const hw_platform_get_humidity_sensor()
{
    return NULL;
}

/* Unused in unit test port */
const LightIntensitySensor *const hw_platform_get_light_intensity_sensor()
{
    return NULL;
}
