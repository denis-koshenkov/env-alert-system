#include "CppUTest/TestHarness.h"
#include "CppUTestExt/OrderedTest.h"

#include "led_notifier.h"
#include "config.h"

TEST_GROUP(LedNotifier){};

/* This test is always the first test to be executed out of a group, so that we can test that initially all
 * notifications are disabled - led_notification_executor_execute is not called for any alert ids. */
TEST_ORDERED(LedNotifier, AllNotificationsInitiallyDisabled, 0)
{
    /* Do not expect any calls to led_notification_executor_execute */

    for (size_t alert_id = 0; alert_id < CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS; alert_id++) {
        led_notifier_notify(alert_id, true);
    }
}
