#include "led_manager.h"
#include "hal/led.h"

void led_manager_add_notification(LedColor led_color, LedPattern led_pattern)
{
    led_set(led_color, led_pattern);
}
