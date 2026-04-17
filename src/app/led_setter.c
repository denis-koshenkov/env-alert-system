#include "led_setter.h"
#include "led_controller.h"

void led_setter_set(LedColor led_color, LedPattern led_pattern)
{
    led_controller_set_color_pattern(led_color, led_pattern);
}

void led_setter_turn_off()
{
    led_controller_turn_off();
}
