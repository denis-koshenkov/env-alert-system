#include "CppUTestExt/MockSupport.h"
#include "mock_led_setter.h"

void led_setter_set(LedColor led_color, LedPattern led_pattern)
{
    mock().actualCall("led_setter_set").withParameter("led_color", led_color).withParameter("led_pattern", led_pattern);
}

void led_setter_turn_off()
{
    mock().actualCall("led_setter_turn_off");
}
