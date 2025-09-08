#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/OrderedTest.h"

#include "led_manager.h"
#include "led_manager_private.h"
#include "config.h"
#include "eas_timer_defs.h"
#include "linked_list_node_allocator.h"
#include "fake_linked_list_node_allocator.h"
#include "eas_time.h"

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
static LedNotification led_notification_3;

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

        /* Led manager calls eas_time_is_equal_or_after inside each timer expiry callback to check if the callback should
         * be accepted or ignore. By default, led manager accepts all callbacks and performs the requried action - hence
         * we set the return value to true here. If a particular test needs to test that the led manager ignores the timer 
         * expiry callback if false is returned, it should set the return value to false inside the test body. */
        fake_eas_time_set_is_equal_or_after_return_value(true);
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
    bool removed_notification1 = led_manager_remove_notification(led_color_1, led_pattern_1);
    /* The last notification is removed. Should turn off the led. */
    bool removed_notification0 = led_manager_remove_notification(led_color_0, led_pattern_0);

    CHECK_TRUE(removed_notification1);
    CHECK_TRUE(removed_notification0);
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
    bool removed_notification = led_manager_remove_notification(led_color, led_pattern);
    CHECK_TRUE(removed_notification);
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
    bool removed_notification = led_manager_remove_notification(led_color, led_pattern);
    CHECK_TRUE(removed_notification);
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
    bool removed_notification2 = led_manager_remove_notification(led_color_2, led_pattern_2);
    /* Currently displaying notification 0, this call removes notification 1. No extra calls to led expected - it should
     * keep displaying notification 0. It should stop the timer, since there is only 1 notification in total. */
    bool removed_notification1 = led_manager_remove_notification(led_color_1, led_pattern_1);
    /* The last notification is removed. Should turn off the led. */
    bool removed_notification0 = led_manager_remove_notification(led_color_0, led_pattern_0);

    CHECK_TRUE(removed_notification2);
    CHECK_TRUE(removed_notification1);
    CHECK_TRUE(removed_notification0);
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
    bool removed_notification1 = led_manager_remove_notification(led_color_1, led_pattern_1);
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
    bool removed_notification2 = led_manager_remove_notification(led_color_2, led_pattern_2);
    /* The last notification is removed. Should turn off the led. */
    bool removed_notification0 = led_manager_remove_notification(led_color_0, led_pattern_0);

    CHECK_TRUE(removed_notification1);
    CHECK_TRUE(removed_notification2);
    CHECK_TRUE(removed_notification0);
}

TEST_ORDERED(LedManager, CurrentlyDisplayedNotificationIsRemoved, 1)
{
    LedColor led_color_0 = LED_COLOR_RED;
    LedPattern led_pattern_0 = LED_PATTERN_STATIC;
    LedColor led_color_1 = LED_COLOR_GREEN;
    LedPattern led_pattern_1 = LED_PATTERN_ALERT;
    LedColor led_color_2 = LED_COLOR_BLUE;
    LedPattern led_pattern_2 = LED_PATTERN_STATIC;
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_1);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_2);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_2).withParameter("led_pattern", led_pattern_2);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_2).withParameter("led_pattern", led_pattern_2);

    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_2);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("eas_timer_stop").withParameter("self", timer);

    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_1);
    mock().expectOneCall("led_turn_off");

    /* Calls led_set */
    led_manager_add_notification(led_color_0, led_pattern_0);
    /* Two notifications added - need to alternate between them. Starts the timer. */
    led_manager_add_notification(led_color_1, led_pattern_1);
    /* No expected calls to led or timer */
    led_manager_add_notification(led_color_2, led_pattern_2);
    /* Notification 0 is being displayed, but is now being removed. This should remove notification 0, start displaying
     * notification 1, and restart the timer to ensure notification 1 is displayed for the whole period. */
    bool removed_notification0 = led_manager_remove_notification(led_color_0, led_pattern_0);
    /* Period for notification 1 expired, should start displaying notification 2 */
    timer_cb(timer_cb_user_data);
    /* Period for notification 2 expired, should start displaying notification 1 */
    timer_cb(timer_cb_user_data);
    /* Period for notification 1 expired, should start displaying notification 2 */
    timer_cb(timer_cb_user_data);

    /* Notification 2 is being disaplyed, but it is now being removed. This should start displaying notification 1 - the
     * last one left. It should also free notification 2 memory and stop the timer, since there is only one notification
     * left. */
    bool removed_notification2 = led_manager_remove_notification(led_color_2, led_pattern_2);
    /* The last notification is removed. Should turn off the led. */
    bool removed_notification1 = led_manager_remove_notification(led_color_1, led_pattern_1);

    CHECK_TRUE(removed_notification0);
    CHECK_TRUE(removed_notification2);
    CHECK_TRUE(removed_notification1);
}

TEST_ORDERED(LedManager, RemoveNotificationReturnsFalseNothingAdded, 1)
{
    LedColor led_color = LED_COLOR_RED;
    LedPattern led_pattern = LED_PATTERN_STATIC;

    bool removed_notification = led_manager_remove_notification(led_color, led_pattern);

    CHECK_FALSE(removed_notification);
}

TEST_ORDERED(LedManager, RemoveNotificationReturnsFalseOneNotificationAdded, 1)
{
    LedColor led_color_0 = LED_COLOR_GREEN;
    LedPattern led_pattern_0 = LED_PATTERN_ALERT;
    LedColor led_color_1 = LED_COLOR_BLUE;
    LedPattern led_pattern_1 = LED_PATTERN_STATIC;
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_0);
    mock().expectOneCall("led_turn_off");

    led_manager_add_notification(led_color_0, led_pattern_0);

    bool removed_notification1 = led_manager_remove_notification(led_color_1, led_pattern_1);

    CHECK_FALSE(removed_notification1);

    /* Clean up */
    bool removed_notification0 = led_manager_remove_notification(led_color_0, led_pattern_0);
    CHECK_TRUE(removed_notification0);
}

TEST_ORDERED(LedManager, RemoveNotificationReturnsFalseTwoNotificationsAdded, 1)
{
    LedColor led_color_0 = LED_COLOR_RED;
    LedPattern led_pattern_0 = LED_PATTERN_ALERT;
    LedColor led_color_1 = LED_COLOR_GREEN;
    LedPattern led_pattern_1 = LED_PATTERN_STATIC;
    LedColor led_color_2 = LED_COLOR_BLUE;
    LedPattern led_pattern_2 = LED_PATTERN_ALERT;
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_1);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    /* Clean up */
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_1);
    mock().expectOneCall("eas_timer_stop").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_0);
    mock().expectOneCall("led_turn_off");

    /* Allocates notification 0 and sets led to notification 0 */
    led_manager_add_notification(led_color_0, led_pattern_0);
    /* Allocates notification 1 and starts notification timer */
    led_manager_add_notification(led_color_1, led_pattern_1);

    bool removed_notification2 = led_manager_remove_notification(led_color_2, led_pattern_2);

    CHECK_FALSE(removed_notification2);

    /* Clean up */
    bool removed_notification1 = led_manager_remove_notification(led_color_1, led_pattern_1);
    bool removed_notification0 = led_manager_remove_notification(led_color_0, led_pattern_0);
    CHECK_TRUE(removed_notification1);
    CHECK_TRUE(removed_notification0);
}

TEST_ORDERED(LedManager, RemoveNotificationReturnsFalseThreeNotificationsAdded, 1)
{
    LedColor led_color_0 = LED_COLOR_RED;
    LedPattern led_pattern_0 = LED_PATTERN_STATIC;
    LedColor led_color_1 = LED_COLOR_GREEN;
    LedPattern led_pattern_1 = LED_PATTERN_ALERT;
    LedColor led_color_2 = LED_COLOR_BLUE;
    LedPattern led_pattern_2 = LED_PATTERN_STATIC;
    LedColor led_color_3 = LED_COLOR_RED;
    LedPattern led_pattern_3 = LED_PATTERN_ALERT;
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_1);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_2);
    /* Clean up */
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_2);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_1);
    mock().expectOneCall("eas_timer_stop").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_0);
    mock().expectOneCall("led_turn_off");

    /* Allocates notification 0 and sets led to notification 0 */
    led_manager_add_notification(led_color_0, led_pattern_0);
    /* Allocates notification 1 and starts notification timer */
    led_manager_add_notification(led_color_1, led_pattern_1);
    /* Allocates notification 2 */
    led_manager_add_notification(led_color_2, led_pattern_2);

    bool removed_notification3 = led_manager_remove_notification(led_color_3, led_pattern_3);

    CHECK_FALSE(removed_notification3);

    /* Clean up */
    bool removed_notification2 = led_manager_remove_notification(led_color_2, led_pattern_2);
    bool removed_notification1 = led_manager_remove_notification(led_color_1, led_pattern_1);
    bool removed_notification0 = led_manager_remove_notification(led_color_0, led_pattern_0);
    CHECK_TRUE(removed_notification2);
    CHECK_TRUE(removed_notification1);
    CHECK_TRUE(removed_notification0);
}

TEST_ORDERED(LedManager, AddOneRemoveOneAddTwoMore, 1)
{
    LedColor led_color_0 = LED_COLOR_GREEN;
    LedPattern led_pattern_0 = LED_PATTERN_STATIC;
    LedColor led_color_1 = LED_COLOR_BLUE;
    LedPattern led_pattern_1 = LED_PATTERN_ALERT;
    LedColor led_color_2 = LED_COLOR_RED;
    LedPattern led_pattern_2 = LED_PATTERN_STATIC;
    LedColor led_color_3 = LED_COLOR_GREEN;
    LedPattern led_pattern_3 = LED_PATTERN_ALERT;
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_1);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_0);
    mock().expectOneCall("eas_timer_stop").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_2);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_3);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_2).withParameter("led_pattern", led_pattern_2);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_3).withParameter("led_pattern", led_pattern_3);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_2).withParameter("led_pattern", led_pattern_2);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_3).withParameter("led_pattern", led_pattern_3);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_3);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_2).withParameter("led_pattern", led_pattern_2);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_2);
    mock().expectOneCall("eas_timer_stop").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_1);
    mock().expectOneCall("led_turn_off");

    /* Allocates notification 0 and sets led to notification 0 */
    led_manager_add_notification(led_color_0, led_pattern_0);
    /* Allocates notification 1 and starts notification timer */
    led_manager_add_notification(led_color_1, led_pattern_1);
    /* Sets led to notification 1 */
    timer_cb(timer_cb_user_data);
    /* Sets led to notification 0 */
    timer_cb(timer_cb_user_data);
    /* Sets led to notification 1 */
    timer_cb(timer_cb_user_data);
    /* Frees notification 0 and stops the timer. Already dispalying notification 1, so led_set is not called. */
    bool removed_notification0 = led_manager_remove_notification(led_color_0, led_pattern_0);
    /* Allocates notification 2 and starts notification timer */
    led_manager_add_notification(led_color_2, led_pattern_2);
    /* Allocates notification 3 */
    led_manager_add_notification(led_color_3, led_pattern_3);
    /* Sets led to notification 2 */
    timer_cb(timer_cb_user_data);
    /* Sets led to notification 3 */
    timer_cb(timer_cb_user_data);
    /* Sets led to notification 1 */
    timer_cb(timer_cb_user_data);
    /* Sets led to notification 2 */
    timer_cb(timer_cb_user_data);
    /* Sets led to notification 3 */
    timer_cb(timer_cb_user_data);
    /* Frees notification 3, sets led to notification 1, and restarts the timer so that notification 1 is displayed for
     * the full period */
    bool removed_notification3 = led_manager_remove_notification(led_color_3, led_pattern_3);
    /* Sets led to notification 2 */
    timer_cb(timer_cb_user_data);
    /* Sets led to notification 1 */
    timer_cb(timer_cb_user_data);
    /* Frees notification 2 and stops the timer */
    bool removed_notification2 = led_manager_remove_notification(led_color_2, led_pattern_2);
    /* Frees notification 1 and turns off the led */
    bool removed_notification1 = led_manager_remove_notification(led_color_1, led_pattern_1);

    CHECK_TRUE(removed_notification0);
    CHECK_TRUE(removed_notification2);
    CHECK_TRUE(removed_notification1);
}

TEST_ORDERED(LedManager, TimerCbFiresWhenRestarted, 1)
{
    LedColor led_color_0 = LED_COLOR_BLUE;
    LedPattern led_pattern_0 = LED_PATTERN_STATIC;
    LedColor led_color_1 = LED_COLOR_RED;
    LedPattern led_pattern_1 = LED_PATTERN_ALERT;
    LedColor led_color_2 = LED_COLOR_GREEN;
    LedPattern led_pattern_2 = LED_PATTERN_STATIC;
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_1);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_2);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_2).withParameter("led_pattern", led_pattern_2);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_1);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_2).withParameter("led_pattern", led_pattern_2);
    mock().expectOneCall("eas_timer_stop").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_2);
    mock().expectOneCall("led_turn_off");

    /* Allocates notification 0 and sets led to notification 0 */
    led_manager_add_notification(led_color_0, led_pattern_0);
    /* Allocates notification 1, starts notification timer, and records timer starting time */
    led_manager_add_notification(led_color_1, led_pattern_1);
    /* Allocates notification 2 */
    led_manager_add_notification(led_color_2, led_pattern_2);
    /* Frees notification 0, sets led to notification 1, restarts the timer (and records starting time) so that
     * notification 1 is displayed for the full period */
    bool removed_notification0 = led_manager_remove_notification(led_color_0, led_pattern_0);
    /* This timer callback was scheduled before we restarted the timer, but it still got executed. Timer callback should
     * ignore the callback since eas_time_is_equal_or_after returns false. */
    fake_eas_time_set_is_equal_or_after_return_value(false);
    timer_cb(timer_cb_user_data);
    fake_eas_time_set_is_equal_or_after_return_value(true);
    /* Gets time and sets led to notification 2 */
    timer_cb(timer_cb_user_data);
    /* Gets time and sets led to notification 1 */
    timer_cb(timer_cb_user_data);
    /* Frees notification 1, sets led to notification 2, and stops the timer */
    bool removed_notification1 = led_manager_remove_notification(led_color_1, led_pattern_1);
    /* Frees notification 2 and turns off the led */
    bool removed_notification2 = led_manager_remove_notification(led_color_2, led_pattern_2);

    CHECK_TRUE(removed_notification0);
    CHECK_TRUE(removed_notification1);
    CHECK_TRUE(removed_notification2);
}
