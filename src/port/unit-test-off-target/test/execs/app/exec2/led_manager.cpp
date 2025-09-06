#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/OrderedTest.h"

#include "led_manager.h"
#include "led_manager_private.h"
#include "config.h"
#include "eas_timer_defs.h"
#include "linked_list_node_allocator.h"
#include "fake_linked_list_node_allocator.h"

/* Notification duration is defined in seconds in the config. The expected timer period is equal to the notification
 * duration, but it is defined in ms. Convert seconds to ms. */
#define LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD (CONFIG_LED_MANAGER_NOTIFICATION_DURATION_SECONDS * 1000)

/* The implementation of eas_timer_create in EasTimer mock object populates these with the notification timer callback
 * and its user data. This is needed in the test so that we can call the callback to simulate the notification duration
 * expiring. */
static EasTimerCb timer_cb;
static void *timer_cb_user_data;

static EasTimer timer = (EasTimer)0x51;

/* Buffers for led notifications to return from mocked calls to led_notification_allocator_alloc */
static LedNotification led_notification_0;
static LedNotification led_notification_1;
static LedNotification led_notification_2;

// clang-format off
TEST_GROUP(LedManager)
{
    void setup() {
        /* Order of expected calls is important for these tests. Fail the test if the expected mock calls do not happen
        in the specified order. */
        mock().strictOrder();

        /* Pass pointers so that the mock object populates them with notification duration timer callback and user data */
        mock().setData("timerCbs", (void *)&timer_cb);
        mock().setData("timerCbsUserData", &timer_cb_user_data);
        mock().setData("numTimerCbs", (unsigned int)1);
        
        /* Linked list will directly call fake linked list node allocator which uses the memory block allocator. If we use 
         * the mock linked list node allocator, the tests will need to expect calls to it. This is not relevant for LedManager
         * tests - they are testing the LedManager behavior, not the way it stores notifications internally. */
        UT_PTR_SET(linked_list_node_allocator_alloc, fake_linked_list_node_allocator_alloc);
        UT_PTR_SET(linked_list_node_allocator_free, fake_linked_list_node_allocator_free);
    }
};
// clang-format on

/* This is the first test - it expects a call to eas_timer_create when the second notification is added. The timer only
 * needs to be created once, so subsequent tests will not expect a call to eas_timer_create. */
TEST_ORDERED(LedManager, PeriodicallySwitchBetweenTwoNotifications, 0)
{
    LedColor led_color_0 = LED_COLOR_BLUE;
    LedPattern led_pattern_0 = LED_PATTERN_STATIC;
    LedColor led_color_1 = LED_COLOR_RED;
    LedPattern led_pattern_1 = LED_PATTERN_ALERT;
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_1);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD)
        .withParameter("periodic", true)
        .ignoreOtherParameters()
        .andReturnValue(timer);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_1);
    mock().expectOneCall("eas_timer_stop").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_0);
    mock().expectOneCall("led_turn_off");

    /* Calls led_set */
    led_manager_add_notification(led_color_0, led_pattern_0);
    /* Two notifications added - need to alternate between them. Creates and starts a timer. */
    led_manager_add_notification(led_color_1, led_pattern_1);
    /* Period for notification 0 expired, should call led_set to start displaying notification 1 */
    timer_cb(timer_cb_user_data);
    /* Period for notification 1 expired, should call led_set to start displaying notification 0 */
    timer_cb(timer_cb_user_data);
    /* Period for notification 0 expired, should call led_set to start displaying notification 1 */
    timer_cb(timer_cb_user_data);
    /* Period for notification 1 expired, should call led_set to start displaying notification 0 */
    timer_cb(timer_cb_user_data);

    /* Currently displaying notification 0, this call removes notification 1. No extra calls to led expected - it should
     * keep displaying notification 0. It should stop the timer, since there is only 1 notification in total. */
    led_manager_remove_notification(led_color_1, led_pattern_1);
    // /* The last notification is removed. Should turn off the led. */
    led_manager_remove_notification(led_color_0, led_pattern_0);
}

TEST_ORDERED(LedManager, AddNotificationSetsLed, 1)
{
    LedColor led_color = LED_COLOR_RED;
    LedPattern led_pattern = LED_PATTERN_STATIC;
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color).withParameter("led_pattern", led_pattern);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_0);
    mock().expectOneCall("led_turn_off");

    /* Calls led_set */
    led_manager_add_notification(led_color, led_pattern);

    /* Clean up */
    /* Calls led_turn_off, since there are now no added notifications - led should be off */
    led_manager_remove_notification(led_color, led_pattern);
}

TEST_ORDERED(LedManager, AddNotificationSetsLedGreenAlert, 1)
{
    LedColor led_color = LED_COLOR_GREEN;
    LedPattern led_pattern = LED_PATTERN_ALERT;
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color).withParameter("led_pattern", led_pattern);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_0);
    mock().expectOneCall("led_turn_off");

    /* Calls led_set */
    led_manager_add_notification(led_color, led_pattern);

    /* Clean up */
    /* Calls led_turn_off, since there are now no added notifications - led should be off */
    led_manager_remove_notification(led_color, led_pattern);
}

TEST_ORDERED(LedManager, PeriodicallySwitchBetweenThreeNotifications, 1)
{
    LedColor led_color_0 = LED_COLOR_GREEN;
    LedPattern led_pattern_0 = LED_PATTERN_STATIC;
    LedColor led_color_1 = LED_COLOR_BLUE;
    LedPattern led_pattern_1 = LED_PATTERN_ALERT;
    LedColor led_color_2 = LED_COLOR_RED;
    LedPattern led_pattern_2 = LED_PATTERN_STATIC;
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_1);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_2);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_2).withParameter("led_pattern", led_pattern_2);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_2).withParameter("led_pattern", led_pattern_2);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_2).withParameter("led_pattern", led_pattern_2);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_2);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_1);
    mock().expectOneCall("eas_timer_stop").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_0);
    mock().expectOneCall("led_turn_off");

    /* Calls led_set */
    led_manager_add_notification(led_color_0, led_pattern_0);
    /* Two notifications added - need to alternate between them. Starts the timer. */
    led_manager_add_notification(led_color_1, led_pattern_1);
    /* No expected calls to led or timer - this should just save notification 2 color and pattern internally to use
     * later. */
    led_manager_add_notification(led_color_2, led_pattern_2);
    /* Period for notification 0 expired, should call led_set to start displaying notification 1 */
    timer_cb(timer_cb_user_data);
    /* Period for notification 1 expired, should call led_set to start displaying notification 2 */
    timer_cb(timer_cb_user_data);
    /* Period for notification 2 expired, should call led_set to start displaying notification 0 */
    timer_cb(timer_cb_user_data);
    /* Period for notification 0 expired, should call led_set to start displaying notification 1 */
    timer_cb(timer_cb_user_data);
    /* Period for notification 1 expired, should call led_set to start displaying notification 2 */
    timer_cb(timer_cb_user_data);
    /* Period for notification 2 expired, should call led_set to start displaying notification 0 */
    timer_cb(timer_cb_user_data);
    /* Period for notification 0 expired, should call led_set to start displaying notification 1 */
    timer_cb(timer_cb_user_data);
    /* Period for notification 1 expired, should call led_set to start displaying notification 2 */
    timer_cb(timer_cb_user_data);
    /* Period for notification 2 expired, should call led_set to start displaying notification 0 */
    timer_cb(timer_cb_user_data);

    /* Clean up */
    /* Currently displaying notification 0, this call removes notification 2. No extra calls to led expected - it should
     * keep displaying notification 0. */
    led_manager_remove_notification(led_color_2, led_pattern_2);
    /* Currently displaying notification 0, this call removes notification 1. No extra calls to led expected - it should
     * keep displaying notification 0. It should stop the timer, since there is only 1 notification in total. */
    led_manager_remove_notification(led_color_1, led_pattern_1);
    /* The last notification is removed. Should turn off the led. */
    led_manager_remove_notification(led_color_0, led_pattern_0);
}

TEST_ORDERED(LedManager, RemoveOneOfSeveralNotifications, 1)
{
    LedColor led_color_0 = LED_COLOR_BLUE;
    LedPattern led_pattern_0 = LED_PATTERN_ALERT;
    LedColor led_color_1 = LED_COLOR_RED;
    LedPattern led_pattern_1 = LED_PATTERN_STATIC;
    LedColor led_color_2 = LED_COLOR_GREEN;
    LedPattern led_pattern_2 = LED_PATTERN_ALERT;
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_1);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_2);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_2).withParameter("led_pattern", led_pattern_2);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_1);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_2).withParameter("led_pattern", led_pattern_2);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_2).withParameter("led_pattern", led_pattern_2);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_2);
    mock().expectOneCall("eas_timer_stop").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_0);
    mock().expectOneCall("led_turn_off");

    /* Calls led_set */
    led_manager_add_notification(led_color_0, led_pattern_0);
    /* Two notifications added - need to alternate between them. Starts the timer. */
    led_manager_add_notification(led_color_1, led_pattern_1);
    /* No expected calls to led or timer - this should just save notification 2 color and pattern internally to use
     * later. */
    led_manager_add_notification(led_color_2, led_pattern_2);
    /* Period for notification 0 expired, should call led_set to start displaying notification 1 */
    timer_cb(timer_cb_user_data);
    /* Period for notification 1 expired, should call led_set to start displaying notification 2 */
    timer_cb(timer_cb_user_data);
    /* Period for notification 2 expired, should call led_set to start displaying notification 0 */
    timer_cb(timer_cb_user_data);
    /* Frees led notification 1 memory */
    led_manager_remove_notification(led_color_1, led_pattern_1);
    /* Period for notification 0 expired. Since notification 1 was removed, this should start displaying
     * notification 2. */
    timer_cb(timer_cb_user_data);
    /* Period for notification 2 expired, should call led_set to start displaying notification 0 */
    timer_cb(timer_cb_user_data);
    /* Period for notification 0 expired, should call led_set to start displaying notification 2 */
    timer_cb(timer_cb_user_data);
    /* Period for notification 2 expired, should call led_set to start displaying notification 0 */
    timer_cb(timer_cb_user_data);

    /* Currently displaying notification 0, this call removes notification 2. No extra calls to led expected - it
     * should keep displaying notification 0. It should stop the timer, since there is only 1 notification in total. */
    led_manager_remove_notification(led_color_2, led_pattern_2);
    /* The last notification is removed. Should turn off the led. */
    led_manager_remove_notification(led_color_0, led_pattern_0);
}
