#include "CppUTest/TestHarness.h"
#include "CppUTestExt/OrderedTest.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/TestAssertPlugin.h"

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

TEST_ORDERED(LedNotifier, EnabledNotificationCallsExecutorId0, 1)
{
    uint8_t alert_id = 0;
    LedColor led_color = LED_COLOR_RED;
    LedPattern led_pattern = LED_PATTERN_STATIC;
    EAS_ASSERT(alert_id < CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS);

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

TEST_ORDERED(LedNotifier, EnabledNotificationCallsExecutorId2, 1)
{
    uint8_t alert_id = 2;
    LedColor led_color = LED_COLOR_GREEN;
    LedPattern led_pattern = LED_PATTERN_ALERT;
    EAS_ASSERT(alert_id < CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS);

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

TEST_ORDERED(LedNotifier, DisabledNotificationDoesNotCallExecutor, 1)
{
    uint8_t alert_id = 1;
    EAS_ASSERT(alert_id < CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS);

    /* This should not call led_notification_executor_execute, since notifications are disabled */
    led_notifier_notify(alert_id, true);
}

TEST_ORDERED(LedNotifier, IsRaisedPropagatedToExecutorCall, 1)
{
    uint8_t alert_id = 0;
    LedColor led_color = LED_COLOR_BLUE;
    LedPattern led_pattern = LED_PATTERN_STATIC;
    EAS_ASSERT(alert_id < CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS);

    mock()
        .expectOneCall("led_notification_executor_execute")
        .withParameter("led_color", led_color)
        .withParameter("led_pattern", led_pattern)
        .withParameter("should_be_displayed", true);
    mock()
        .expectOneCall("led_notification_executor_execute")
        .withParameter("led_color", led_color)
        .withParameter("led_pattern", led_pattern)
        .withParameter("should_be_displayed", false);

    led_notifier_enable_notifications(alert_id, led_color, led_pattern);
    /* Calls led_notification_executor_execute with should_be_displayed == true */
    led_notifier_notify(alert_id, true);
    /* Calls led_notification_executor_execute with should_be_displayed == false */
    led_notifier_notify(alert_id, false);

    /* Clean up */
    led_notifier_disable_notifications(alert_id);
}

TEST_ORDERED(LedNotifier, DisablePreviouslyEnabledNotificationDoesNotCallExecutor, 1)
{
    uint8_t alert_id = 1;
    LedColor led_color = LED_COLOR_RED;
    LedPattern led_pattern = LED_PATTERN_ALERT;
    EAS_ASSERT(alert_id < CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS);

    led_notifier_enable_notifications(alert_id, led_color, led_pattern);
    led_notifier_disable_notifications(alert_id);
    /* This should not call led_notification_executor_execute, since notifications are now disabled */
    led_notifier_notify(alert_id, true);
}

TEST_ORDERED(LedNotifier, EnablePreviouslyDisabledNotificationCallsExecutor, 1)
{
    uint8_t alert_id = 0;
    LedColor led_color = LED_COLOR_GREEN;
    LedPattern led_pattern = LED_PATTERN_STATIC;
    EAS_ASSERT(alert_id < CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS);

    mock()
        .expectOneCall("led_notification_executor_execute")
        .withParameter("led_color", led_color)
        .withParameter("led_pattern", led_pattern)
        .withParameter("should_be_displayed", true);

    led_notifier_enable_notifications(alert_id, led_color, led_pattern);
    led_notifier_disable_notifications(alert_id);
    led_notifier_enable_notifications(alert_id, led_color, led_pattern);
    /* Should call led_notification_executor_execute */
    led_notifier_notify(alert_id, true);

    /* Clean up */
    led_notifier_disable_notifications(alert_id);
}

TEST_ORDERED(LedNotifier, EnableNotificationAssertsIfAlertIdInvalid, 1)
{
    uint8_t alert_id = CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS;
    LedColor led_color = LED_COLOR_BLUE;
    LedPattern led_pattern = LED_PATTERN_ALERT;
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("is_valid_alert_id(alert_id)", "led_notifier_enable_notifications");

    led_notifier_enable_notifications(alert_id, led_color, led_pattern);
}

TEST_ORDERED(LedNotifier, DisableNotificationAssertsIfAlertIdInvalid, 1)
{
    uint8_t alert_id = CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS;
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("is_valid_alert_id(alert_id)", "led_notifier_disable_notifications");

    led_notifier_disable_notifications(alert_id);
}

TEST_ORDERED(LedNotifier, NotifyAssertsIfAlertIdInvalid, 1)
{
    uint8_t alert_id = CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS;
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("is_valid_alert_id(alert_id)", "led_notifier_notify");

    led_notifier_notify(alert_id, false);
}
