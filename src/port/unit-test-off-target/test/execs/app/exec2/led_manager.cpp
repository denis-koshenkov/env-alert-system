#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/OrderedTest.h"

#include "led_manager.h"
#include "led_manager_private.h"
#include "config.h"
#include "eas_timer_defs.h"
#include "linked_list_node_allocator.h"
#include "fake_linked_list_node_allocator.h"
#include "eas_current_time.h"

/* Notification duration is defined in seconds in the config. The expected timer period is equal to the notification
 * duration, but it is defined in ms. Convert seconds to ms. */
#define LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD (CONFIG_LED_MANAGER_NOTIFICATION_DURATION_SECONDS * 1000)

/* If at least LED_MANAGER_TEST_IGNORE_TIMER_PERIOD_MS pass from the moment the timer is started, the expiry callback
 * should be accepted. Otherwise, the expiry callback should be ignored. */
#define LED_MANAGER_TEST_IGNORE_TIMER_PERIOD_MS                                                                        \
    (LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD - CONFIG_LED_MANAGER_IGNORE_TIMER_MARGIN_MS)

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

static EasTime current_time = 0;

void reset_current_time()
{
    current_time = 0;
    fake_eas_current_time_set(current_time);
}

void advance_current_time_by(size_t offset_ms)
{
    current_time += offset_ms;
    fake_eas_current_time_set(current_time);
}

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

        /* Time starts from 0 at the beginning of each test. Each test can advance time however it needs. */
        reset_current_time();
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
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 0 expired, should call led_set to start displaying notification 1 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 1 expired, should call led_set to start displaying notification 0 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 0 expired, should call led_set to start displaying notification 1 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
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
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 0 expired, should call led_set to start displaying notification 1 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 1 expired, should call led_set to start displaying notification 2 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 2 expired, should call led_set to start displaying notification 0 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 0 expired, should call led_set to start displaying notification 1 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 1 expired, should call led_set to start displaying notification 2 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 2 expired, should call led_set to start displaying notification 0 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 0 expired, should call led_set to start displaying notification 1 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 1 expired, should call led_set to start displaying notification 2 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
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

    /* Just so that one of the tests does not start time from 0 */
    advance_current_time_by(10500);
    /* Calls led_set */
    led_manager_add_notification(led_color_0, led_pattern_0);
    /* Two notifications added - need to alternate between them. Starts the timer. */
    led_manager_add_notification(led_color_1, led_pattern_1);
    /* No expected calls to led or timer - this should just save notification 2 color and pattern internally to use
     * later. */
    led_manager_add_notification(led_color_2, led_pattern_2);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 0 expired, should call led_set to start displaying notification 1 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 1 expired, should call led_set to start displaying notification 2 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 2 expired, should call led_set to start displaying notification 0 */
    timer_cb(timer_cb_user_data);
    /* Frees led notification 1 memory */
    bool removed_notification1 = led_manager_remove_notification(led_color_1, led_pattern_1);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 0 expired. Since notification 1 was removed, this should start displaying
     * notification 2. */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 2 expired, should call led_set to start displaying notification 0 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 0 expired, should call led_set to start displaying notification 2 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
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
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 1 expired, should start displaying notification 2 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Period for notification 2 expired, should start displaying notification 1 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
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
    advance_current_time_by(LED_MANAGER_TEST_IGNORE_TIMER_PERIOD_MS);
    /* Callback should still be accepted if exactly timer ignore period elapsed since timer is started */
    /* Sets led to notification 1 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 0 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 1 */
    timer_cb(timer_cb_user_data);
    /* Frees notification 0 and stops the timer. Already dispalying notification 1, so led_set is not called. */
    bool removed_notification0 = led_manager_remove_notification(led_color_0, led_pattern_0);
    /* Allocates notification 2 and starts notification timer */
    led_manager_add_notification(led_color_2, led_pattern_2);
    /* Allocates notification 3 */
    led_manager_add_notification(led_color_3, led_pattern_3);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 2 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 3 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 1 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 2 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 3 */
    timer_cb(timer_cb_user_data);
    /* Frees notification 3, sets led to notification 1, and restarts the timer so that notification 1 is displayed for
     * the full period */
    bool removed_notification3 = led_manager_remove_notification(led_color_3, led_pattern_3);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 2 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
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
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_2).withParameter("led_pattern", led_pattern_2);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_1);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_2).withParameter("led_pattern", led_pattern_2);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_2).withParameter("led_pattern", led_pattern_2);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_2).withParameter("led_pattern", led_pattern_2);
    mock().expectOneCall("eas_timer_stop").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_2);
    mock().expectOneCall("led_turn_off");

    /* Allocates notification 0 and sets led to notification 0 */
    led_manager_add_notification(led_color_0, led_pattern_0);
    /* Allocates notification 1 and starts notification timer */
    led_manager_add_notification(led_color_1, led_pattern_1);
    /* Allocates notification 2 */
    led_manager_add_notification(led_color_2, led_pattern_2);
    /* Frees notification 0, sets led to notification 1, restarts the timer so that notification 1 is displayed for the
     * full period */
    bool removed_notification0_1 = led_manager_remove_notification(led_color_0, led_pattern_0);
    advance_current_time_by(1);
    /* This timer callback was scheduled before we restarted the timer, but it still got executed. Only 1 ms passed
     * since starting the timer, so this callback should be ignored. */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 2 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 1 */
    timer_cb(timer_cb_user_data);
    /* Allocates notification 0 */
    led_manager_add_notification(led_color_0, led_pattern_0);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 2 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 0 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 1 */
    timer_cb(timer_cb_user_data);
    /* Frees notification 1, sets led to notification 2, restarts the timer so that notification 2 is displayed for the
     * full period */
    bool removed_notification1 = led_manager_remove_notification(led_color_1, led_pattern_1);
    /* Here the unexpected timer callback fires 1 ms before the ignore timer period expires, so seconds after the timer
     * was restarted. This is not a realistic scenario - if a callback fires before the expected timer period elapses,
     * it would always be within milliseconds after the timer is restarted. The callback would be fired because it was
     * already scheduled to be executed before the timer was restarted. However, this test is here to make sure that the
     * led manager actually respects the ignore period that is calculated from
     * CONFIG_LED_MANAGER_NOTIFICATION_DURATION_SECONDS and CONFIG_LED_MANAGER_IGNORE_TIMER_MARGIN_MS configs.
     */
    advance_current_time_by(LED_MANAGER_TEST_IGNORE_TIMER_PERIOD_MS - 1);
    /* Should be ignored */
    timer_cb(timer_cb_user_data);
    /* Now, LED_MANAGER_TEST_IGNORE_TIMER_PERIOD_MS elapsed since starting the timer */
    advance_current_time_by(1);
    /* Should be accepted and set the led to notification 0 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 2 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 0 */
    timer_cb(timer_cb_user_data);

    /* Frees notification 0, sets led to notification 2, and stops the timer */
    bool removed_notification0_2 = led_manager_remove_notification(led_color_0, led_pattern_0);
    /* Frees notification 2 and turns off the led */
    bool removed_notification2 = led_manager_remove_notification(led_color_2, led_pattern_2);

    CHECK_TRUE(removed_notification0_1);
    CHECK_TRUE(removed_notification1);
    CHECK_TRUE(removed_notification0_2);
    CHECK_TRUE(removed_notification2);
}

TEST_ORDERED(LedManager, TimerCbFiresWhenStopped, 1)
{
    LedColor led_color_0 = LED_COLOR_BLUE;
    LedPattern led_pattern_0 = LED_PATTERN_ALERT;
    LedColor led_color_1 = LED_COLOR_RED;
    LedPattern led_pattern_1 = LED_PATTERN_STATIC;
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_1);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_1);
    mock().expectOneCall("eas_timer_stop").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_0);
    mock().expectOneCall("led_turn_off");

    /* Allocates notification 0 and sets led to notification 0 */
    led_manager_add_notification(led_color_0, led_pattern_0);
    /* Allocates notification 1 and starts notification timer */
    led_manager_add_notification(led_color_1, led_pattern_1);
    /* Timer period almost expired when we remove notification 1 */
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD - 1);
    /* Frees notification 1 and stops the timer */
    bool removed_notification1 = led_manager_remove_notification(led_color_1, led_pattern_1);
    advance_current_time_by(1);
    /* The timer was stopped right before its expiry time, so its expiry callback still gets executed. Led manager is
     * expected to detect that the timer is stopped and ignore this callback. */
    timer_cb(timer_cb_user_data);
    /* Frees notification 0 and turns off the led */
    bool removed_notification0 = led_manager_remove_notification(led_color_0, led_pattern_0);

    CHECK_TRUE(removed_notification1);
    CHECK_TRUE(removed_notification0);
}

TEST_ORDERED(LedManager, SeveralNotificationsSameColorPattern, 1)
{
    LedColor led_color = LED_COLOR_GREEN;
    LedPattern led_pattern = LED_PATTERN_ALERT;
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color).withParameter("led_pattern", led_pattern);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_1);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    mock().expectOneCall("led_set").withParameter("led_color", led_color).withParameter("led_pattern", led_pattern);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_2);
    mock().expectOneCall("led_set").withParameter("led_color", led_color).withParameter("led_pattern", led_pattern);
    mock().expectOneCall("led_set").withParameter("led_color", led_color).withParameter("led_pattern", led_pattern);
    mock().expectOneCall("led_set").withParameter("led_color", led_color).withParameter("led_pattern", led_pattern);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_0);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_1);
    mock().expectOneCall("led_set").withParameter("led_color", led_color).withParameter("led_pattern", led_pattern);
    mock().expectOneCall("eas_timer_stop").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_2);
    mock().expectOneCall("led_turn_off");

    /* Allocates notification 0 and sets led to notification 0 */
    led_manager_add_notification(led_color, led_pattern);
    /* Allocates notification 1 and starts notification timer */
    led_manager_add_notification(led_color, led_pattern);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 1 */
    timer_cb(timer_cb_user_data);
    /* Allocates notification 2 */
    led_manager_add_notification(led_color, led_pattern);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 2 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 0 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 1 */
    timer_cb(timer_cb_user_data);
    /* Frees notification 0. Led manager will remove the first notification in the list that has the requested color and
     * pattern. */
    bool removed_notification0 = led_manager_remove_notification(led_color, led_pattern);
    /* Frees notification 1, since it is now the first one in the list with the requested color and pattern. Sets led to
     * notification 0. Stops the timer. */
    bool removed_notification1 = led_manager_remove_notification(led_color, led_pattern);
    /* Frees notification 2 and turns off the led */
    bool removed_notification2 = led_manager_remove_notification(led_color, led_pattern);

    CHECK_TRUE(removed_notification0);
    CHECK_TRUE(removed_notification1);
    CHECK_TRUE(removed_notification2);
}

TEST_ORDERED(LedManager, SameColorPatternNotificationsMixedWithOtherColorPatterns, 1)
{
    LedColor led_color_red = LED_COLOR_RED;
    LedPattern led_pattern_static = LED_PATTERN_STATIC;
    LedColor led_color_1 = LED_COLOR_GREEN;
    LedPattern led_pattern_1 = LED_PATTERN_ALERT;
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_0);
    /* Notification 0 */
    mock()
        .expectOneCall("led_set")
        .withParameter("led_color", led_color_red)
        .withParameter("led_pattern", led_pattern_static);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_1);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_2);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_3);
    /* Notification 1 */
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    /* Notification 2 */
    mock()
        .expectOneCall("led_set")
        .withParameter("led_color", led_color_red)
        .withParameter("led_pattern", led_pattern_static);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_0);
    /* Notification 3 */
    mock()
        .expectOneCall("led_set")
        .withParameter("led_color", led_color_red)
        .withParameter("led_pattern", led_pattern_static);
    /* Notification 1 */
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    /* Notification 2 */
    mock()
        .expectOneCall("led_set")
        .withParameter("led_color", led_color_red)
        .withParameter("led_pattern", led_pattern_static);
    /* Notification 3 */
    mock()
        .expectOneCall("led_set")
        .withParameter("led_color", led_color_red)
        .withParameter("led_pattern", led_pattern_static);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_2);
    /* Notification 1 */
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    /* Notification 3 */
    mock()
        .expectOneCall("led_set")
        .withParameter("led_color", led_color_red)
        .withParameter("led_pattern", led_pattern_static);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_1);
    mock().expectOneCall("eas_timer_stop").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_3);
    mock().expectOneCall("led_turn_off");

    /* Allocates notification 0 and sets led to notification 0 */
    led_manager_add_notification(led_color_red, led_pattern_static);
    /* Allocates notification 1 and starts notification timer */
    led_manager_add_notification(led_color_1, led_pattern_1);
    /* Allocates notification 2 */
    led_manager_add_notification(led_color_red, led_pattern_static);
    /* Allocates notification 3 */
    led_manager_add_notification(led_color_red, led_pattern_static);

    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 1 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 2 */
    timer_cb(timer_cb_user_data);
    /* Frees notification 0 */
    bool removed_notification0 = led_manager_remove_notification(led_color_red, led_pattern_static);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 3 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 1 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 2 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 3 */
    timer_cb(timer_cb_user_data);
    /* Frees notification 2 */
    bool removed_notification2 = led_manager_remove_notification(led_color_red, led_pattern_static);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 1 */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 3 */
    timer_cb(timer_cb_user_data);
    /* Frees notification 1 and stops the timer */
    bool removed_notification1 = led_manager_remove_notification(led_color_1, led_pattern_1);
    /* Frees notification 3 and turns off the led */
    bool removed_notification3 = led_manager_remove_notification(led_color_red, led_pattern_static);

    CHECK_TRUE(removed_notification0);
    CHECK_TRUE(removed_notification2);
    CHECK_TRUE(removed_notification1);
    CHECK_TRUE(removed_notification3);
}

TEST_ORDERED(LedManager, StoppedTimerCbFiresWhenStoppedAndImmediatelyStarted, 1)
{
    LedColor led_color_0 = LED_COLOR_RED;
    LedPattern led_pattern_0 = LED_PATTERN_STATIC;
    LedColor led_color_1 = LED_COLOR_BLUE;
    LedPattern led_pattern_1 = LED_PATTERN_ALERT;
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_1);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("eas_timer_stop").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_alloc").andReturnValue(&led_notification_0);
    mock().expectOneCall("eas_timer_start").withParameter("self", timer);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_0).withParameter("led_pattern", led_pattern_0);
    mock().expectOneCall("led_set").withParameter("led_color", led_color_1).withParameter("led_pattern", led_pattern_1);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_0);
    mock().expectOneCall("eas_timer_stop").withParameter("self", timer);
    mock().expectOneCall("led_notification_allocator_free").withParameter("led_notification", &led_notification_1);
    mock().expectOneCall("led_turn_off");

    /* Allocates notification 0 and sets led to notification 0 */
    led_manager_add_notification(led_color_0, led_pattern_0);
    /* Allocates notification 1 and starts notification timer */
    led_manager_add_notification(led_color_1, led_pattern_1);
    /* Timer period almost expired when we remove notification 0 */
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD - 1);
    /* Frees notification 0, sets led to notification 1, and stops the timer */
    bool removed_notification0_1 = led_manager_remove_notification(led_color_0, led_pattern_0);
    /* Allocates notification 0 and starts the timer */
    led_manager_add_notification(led_color_0, led_pattern_0);
    advance_current_time_by(1);
    /* The timer was stopped right before its expiry time, so its expiry callback still gets executed. The timer has
     * already been started since, but we get the callback from the old timer run, the one that was stopped. This
     * callback should be ignored. */
    timer_cb(timer_cb_user_data);
    advance_current_time_by(LED_MANAGER_TEST_EXPECTED_TIMER_PERIOD);
    /* Sets led to notification 0 - this is the expiry callback of the current timer, not the old one */
    timer_cb(timer_cb_user_data);
    /* Sets led to notification 1 */
    timer_cb(timer_cb_user_data);

    /* Frees notification 0 and stops the timer */
    bool removed_notification0_2 = led_manager_remove_notification(led_color_0, led_pattern_0);
    /* Frees notification 1 and turns off the led */
    bool removed_notification1 = led_manager_remove_notification(led_color_1, led_pattern_1);

    CHECK_TRUE(removed_notification0_1);
    CHECK_TRUE(removed_notification0_2);
    CHECK_TRUE(removed_notification1);
}
