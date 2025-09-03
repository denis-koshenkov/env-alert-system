#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "led_manager.h"

TEST_GROUP(LedManager){};

TEST(LedManager, AddNotificationSetsLed)
{
    LedColor led_color = LED_COLOR_RED;
    LedPattern led_pattern = LED_PATTERN_STATIC;
    mock().expectOneCall("led_set").withParameter("led_color", led_color).withParameter("led_pattern", led_pattern);
    mock().expectOneCall("led_turn_off");

    /* Calls led_set */
    led_manager_add_notification(led_color, led_pattern);

    /* Clean up */
    /* Calls led_turn_off, since there are now no added notifications - led should be off */
    led_manager_remove_notification(led_color, led_pattern);
}

TEST(LedManager, AddNotificationSetsLedGreenAlert)
{
    LedColor led_color = LED_COLOR_GREEN;
    LedPattern led_pattern = LED_PATTERN_ALERT;
    mock().expectOneCall("led_set").withParameter("led_color", led_color).withParameter("led_pattern", led_pattern);
    mock().expectOneCall("led_turn_off");

    /* Calls led_set */
    led_manager_add_notification(led_color, led_pattern);

    /* Clean up */
    /* Calls led_turn_off, since there are now no added notifications - led should be off */
    led_manager_remove_notification(led_color, led_pattern);
}
