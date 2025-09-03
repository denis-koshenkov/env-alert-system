#include "CppUTestExt/MockSupport.h"
#include "hal/led.h"

void led_set(LedColor led_color, LedPattern led_pattern)
{
    mock().actualCall("led_set").withParameter("led_color", led_color).withParameter("led_pattern", led_pattern);
}
