#include "virtual_led_nrf52840.h"

static void led_set(LedColor led_color);
static void led_turn_off();

static Led led = {
    .set = led_set,
    .turn_off = led_turn_off,
};

static void led_set(LedColor led_color)
{
}

static void led_turn_off()
{
}

Nrf52840LedVirtualInterfaces virtual_led_nrf52840_initialize()
{
    return (Nrf52840LedVirtualInterfaces){.led = &led};
}
