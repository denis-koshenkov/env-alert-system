#include "led_controller.h"
#include "hw_platform.h"

void led_controller_turn_off()
{
    hw_platform_get_led()->turn_off();
}
