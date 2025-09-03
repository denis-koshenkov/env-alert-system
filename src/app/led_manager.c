#include "led_manager.h"
#include "hal/hw_platform.h"

void led_manager_add_notification(LedColor led_color, LedPattern led_pattern)
{
    hw_platform_get_led()->set(led_color, led_pattern);
}

void led_manager_remove_notification(LedColor led_color, LedPattern led_pattern)
{
    hw_platform_get_led()->turn_off(led_color, led_pattern);
}
