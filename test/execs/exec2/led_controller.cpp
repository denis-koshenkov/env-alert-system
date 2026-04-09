#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "led_controller.h"

TEST_GROUP(LedController){};

TEST(LedController, TurnOffSetsLedToOff)
{
    mock().expectOneCall("led_turn_off");
    led_controller_turn_off();
}

TEST(LedController, SetStaticPatternSetLedRed)
{
    mock().expectOneCall("led_set").withParameter("led_color", LED_COLOR_RED);
    led_controller_set_color_pattern(LED_COLOR_RED, LED_PATTERN_STATIC);
}

TEST(LedController, SetStaticPatternSetLedGreen)
{
    mock().expectOneCall("led_set").withParameter("led_color", LED_COLOR_GREEN);
    led_controller_set_color_pattern(LED_COLOR_GREEN, LED_PATTERN_STATIC);
}
