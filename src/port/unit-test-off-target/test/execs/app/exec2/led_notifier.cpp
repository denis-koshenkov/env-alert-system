#include "CppUTest/TestHarness.h"
#include "CppUTestExt/OrderedTest.h"
#include "CppUTestExt/MockSupport.h"

#include "led_notifier.h"
#include "config.h"
#include "eas_assert.h"

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

TEST_ORDERED(LedNotifier, EnabledNotificationCallsSenderId0, 1)
{
    uint8_t alert_id = 0;
    LedColor led_color = LED_COLOR_RED;
    LedPattern led_pattern = LED_PATTERN_STATIC;
    EAS_ASSERT(alert_id < CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS);

    mock()
        .expectOneCall("led_notification_executor_execute")
        .withParameter("led_color", led_color)
        .withParameter("led_pattern", led_pattern)
        .withParameter("should_be_displayed", true);

    led_notifier_enable_notifications(alert_id, led_color, led_pattern);
    /* This calls led_notification_executor_execute, since notifications are now enabled */
    led_notifier_notify(alert_id, true);

    /* Clean up */
    led_notifier_disable_notifications(alert_id);
}

TEST_ORDERED(LedNotifier, EnabledNotificationCallsSenderId2, 1)
{
    uint8_t alert_id = 2;
    LedColor led_color = LED_COLOR_GREEN;
    LedPattern led_pattern = LED_PATTERN_ALERT;
    EAS_ASSERT(alert_id < CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS);

    mock()
        .expectOneCall("led_notification_executor_execute")
        .withParameter("led_color", led_color)
        .withParameter("led_pattern", led_pattern)
        .withParameter("should_be_displayed", true);

    led_notifier_enable_notifications(alert_id, led_color, led_pattern);
    /* This calls led_notification_executor_execute, since notifications are now enabled */
    led_notifier_notify(alert_id, true);

    /* Clean up */
    led_notifier_disable_notifications(alert_id);
}
