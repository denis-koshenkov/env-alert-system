#include "hal/hw_platform.h"
#include "led_nrf52840.h"

const Led *const hw_platform_get_led()
{
    return led_nrf52840_get();
}
