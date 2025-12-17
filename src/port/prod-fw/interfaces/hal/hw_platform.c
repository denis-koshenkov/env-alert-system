#include <stddef.h>

#include "hw_platform.h"
#include "led_nrf52840.h"
#include "eas_assert.h"
#include "virtual_sht31.h"

static const TemperatureSensor *temperature_sensor = NULL;
static const HumiditySensor *humidity_sensor = NULL;

void hw_platform_init()
{
    SHT31VirtualInterfaces interfaces = virtual_sht31_initialize();
    temperature_sensor = interfaces.temperature_sensor;
    humidity_sensor = interfaces.humidity_sensor;
}

const Led *const hw_platform_get_led()
{
    return led_nrf52840_get();
}

const TemperatureSensor *const hw_platform_get_temperature_sensor()
{
    EAS_ASSERT(temperature_sensor);
    return temperature_sensor;
}

const HumiditySensor *const hw_platform_get_humidity_sensor()
{
    EAS_ASSERT(humidity_sensor);
    return humidity_sensor;
}
