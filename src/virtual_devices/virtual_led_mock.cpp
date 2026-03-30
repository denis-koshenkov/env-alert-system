#include "CppUTestExt/MockSupport.h"
#include "virtual_led_mock.h"

static void led_set(LedColor led_color);
static void led_turn_off();

static Led led = {
    .set = led_set,
    .turn_off = led_turn_off,
};

static void led_set(LedColor led_color)
{
    mock().actualCall("led_set").withParameter("led_color", led_color);
}

static void led_turn_off()
{
    mock().actualCall("led_turn_off");
}

const Led *const virtual_led_mock_get()
{
    return &led;
}
