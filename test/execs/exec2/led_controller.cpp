#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "led_controller.h"

TEST_GROUP(LedController){};

TEST(LedController, TurnOffSetsLedToOff)
{
    mock().expectOneCall("led_turn_off");
    led_controller_turn_off();
}
