#include "led_nrf52840.h"

static void led_set(LedColor led_color, LedPattern led_pattern);
static void led_turn_off();

static Led led = {
    .set = led_set,
    .turn_off = led_turn_off,
};

static void led_set(LedColor led_color, LedPattern led_pattern)
{
}

static void led_turn_off()
{
}

const Led *const led_nrf52840_get()
{
    return &led;
}
