#include "led_manager.h"
#include "hal/led.h"

void led_manager_add_notification(LedColor led_color, LedPattern led_pattern)
{
    led_set(LED_COLOR_RED, LED_PATTERN_STATIC);
}
