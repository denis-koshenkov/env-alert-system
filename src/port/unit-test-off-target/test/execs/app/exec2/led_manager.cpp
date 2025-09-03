#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "led_manager.h"

TEST_GROUP(LedManager){};

TEST(LedManager, AddNotificationSetsLed)
{
    LedColor led_color = LED_COLOR_RED;
    LedPattern led_pattern = LED_PATTERN_STATIC;
    mock().expectOneCall("led_set").withParameter("led_color", led_color).withParameter("led_pattern", led_pattern);

    led_manager_add_notification(led_color, led_pattern);
}
