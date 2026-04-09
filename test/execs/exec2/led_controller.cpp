#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/TestAssertPlugin.h"
#include "CppUTestExt/OrderedTest.h"

#include "led_controller.h"
#include "eas_timer_defs.h"

#define LED_CONTROLLER_TEST_TIMER_PERIOD 300

/* The implementation of eas_timer_create in EasTimer mock object populates these with the timer callback and its user
 * data. This is needed in the test so that we can call the callback to simulate the timer expiration. */
static EasTimerCb timer_cb;
static void *timer_cb_user_data;

static EasTimer timer = (EasTimer)0x28;

// clang-format off
TEST_GROUP(LedController)
{
    void setup() {
        /* Order of expected calls is important for these tests. Fail the test if the expected mock calls do not happen
        in the specified order. */
        mock().strictOrder();

        /* Pass pointers so that the mock object populates them with timer callback and user data */
        mock().setData("timerCbs", (void *)&timer_cb);
        mock().setData("timerCbsUserData", &timer_cb_user_data);
        mock().setData("numTimerCbs", (unsigned int)1);        
    }
};
// clang-format on

/* This is the first test - it expects a call to eas_timer_create when the alert pattern is set. The timer only needs to
 * be created once, so subsequent tests will not expect a call to eas_timer_create. */
TEST_ORDERED(LedController, AlertPatternRed, 0)
{
    /* Called by led_controller_set_color_pattern */
    mock().expectOneCall("led_set").withParameter("led_color", LED_COLOR_RED);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", LED_CONTROLLER_TEST_TIMER_PERIOD)
        .withParameter("periodic", true)
        .ignoreOtherParameters()
        .andReturnValue(timer);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    /* Called by timer_cb */
    mock().expectOneCall("led_turn_off");
    /* Called by timer_cb */
    mock().expectOneCall("led_set").withParameter("led_color", LED_COLOR_RED);
    /* Called by timer_cb */
    mock().expectOneCall("led_turn_off");

    led_controller_set_color_pattern(LED_COLOR_RED, LED_PATTERN_ALERT);
    timer_cb(timer_cb_user_data);
    timer_cb(timer_cb_user_data);
    timer_cb(timer_cb_user_data);
}

TEST_ORDERED(LedController, TurnOffSetsLedToOff, 1)
{
    mock().expectOneCall("led_turn_off");
    led_controller_turn_off();
}

TEST_ORDERED(LedController, SetStaticPatternSetLedRed, 1)
{
    mock().expectOneCall("led_set").withParameter("led_color", LED_COLOR_RED);
    led_controller_set_color_pattern(LED_COLOR_RED, LED_PATTERN_STATIC);
}

TEST_ORDERED(LedController, SetStaticPatternSetLedGreen, 1)
{
    mock().expectOneCall("led_set").withParameter("led_color", LED_COLOR_GREEN);
    led_controller_set_color_pattern(LED_COLOR_GREEN, LED_PATTERN_STATIC);
}

TEST_ORDERED(LedController, InvalidPattern, 1)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("controller_pattern", "led_controller_set_color_pattern");

    LedPattern invalid_pattern = (LedPattern)0x55;
    led_controller_set_color_pattern(LED_COLOR_BLUE, invalid_pattern);
}
