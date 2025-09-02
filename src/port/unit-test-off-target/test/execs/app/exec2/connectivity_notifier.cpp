#include "CppUTest/TestHarness.h"
#include "CppUTestExt/OrderedTest.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "connectivity_notifier.h"
#include "config.h"
#include "eas_assert.h"

TEST_GROUP(ConnectivityNotifier){};

/* This test is always the first test to be executed out of a group, so that we can test that initially all
 * notifications are unregistered - connectivity_notification_sender function is not called for any alert ids. */
TEST_ORDERED(ConnectivityNotifier, AllNotificationsInitiallyUnregistered, 0)
{
    /* Do not expect any calls to connectivity_notification_sender */

    for (size_t alert_id = 0; alert_id < CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS; alert_id++) {
        connectivity_notifier_notify(alert_id, true);
    }
}

TEST_ORDERED(ConnectivityNotifier, RegisteredNotificationCallsSenderId0, 1)
{
    uint8_t alert_id = 0;
    EAS_ASSERT(alert_id < CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS);

    mock()
        .expectOneCall("connectivity_notification_sender_send")
        .withParameter("alert_id", alert_id)
        .withParameter("is_raised", true);

    connectivity_notifier_register_notification(alert_id);
    /* This calls connectivity_notification_sender_send, since the notification is now registered */
    connectivity_notifier_notify(alert_id, true);

    /* Clean up */
    connectivity_notifier_unregister_notification(alert_id);
}

TEST_ORDERED(ConnectivityNotifier, RegisteredNotificationCallsSenderId1, 1)
{
    uint8_t alert_id = 1;
    EAS_ASSERT(alert_id < CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS);

    mock()
        .expectOneCall("connectivity_notification_sender_send")
        .withParameter("alert_id", alert_id)
        .withParameter("is_raised", true);

    connectivity_notifier_register_notification(alert_id);
    /* This calls connectivity_notification_sender_send, since the notification is now registered */
    connectivity_notifier_notify(alert_id, true);

    /* Clean up */
    connectivity_notifier_unregister_notification(alert_id);
}

TEST_ORDERED(ConnectivityNotifier, UnregisteredNotificationDoesNotCallSender, 1)
{
    uint8_t alert_id = 2;
    EAS_ASSERT(alert_id < CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS);

    /* This should not call connectivity_notification_sender_send, since the notification is unregistered */
    connectivity_notifier_notify(alert_id, true);
}

TEST_ORDERED(ConnectivityNotifier, UnregisterPreviouslyRegisteredNotificationDoesNotCallSender, 1)
{
    uint8_t alert_id = 1;
    EAS_ASSERT(alert_id < CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS);

    connectivity_notifier_register_notification(alert_id);
    connectivity_notifier_unregister_notification(alert_id);

    /* This should not call connectivity_notification_sender_send, since the notification is now unregistered */
    connectivity_notifier_notify(alert_id, true);
}

TEST_ORDERED(ConnectivityNotifier, IsRaisedTruePropagated, 1)
{
    uint8_t alert_id = 0;
    bool is_raised = true;
    EAS_ASSERT(alert_id < CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS);
    mock()
        .expectOneCall("connectivity_notification_sender_send")
        .withParameter("alert_id", alert_id)
        .withParameter("is_raised", is_raised);

    connectivity_notifier_register_notification(alert_id);
    connectivity_notifier_notify(alert_id, is_raised);
}

TEST_ORDERED(ConnectivityNotifier, IsRaisedFalsePropagated, 1)
{
    uint8_t alert_id = 0;
    bool is_raised = false;
    EAS_ASSERT(alert_id < CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS);
    mock()
        .expectOneCall("connectivity_notification_sender_send")
        .withParameter("alert_id", alert_id)
        .withParameter("is_raised", is_raised);

    connectivity_notifier_register_notification(alert_id);
    connectivity_notifier_notify(alert_id, is_raised);
}

TEST_ORDERED(ConnectivityNotifier, RegisterNotificationAssertsIfAlertIdInvalid, 1)
{
    uint8_t alert_id = CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS;
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("is_valid_alert_id(alert_id)", "connectivity_notifier_register_notification");

    connectivity_notifier_register_notification(alert_id);
}

TEST_ORDERED(ConnectivityNotifier, UnregisterNotificationAssertsIfAlertIdInvalid, 1)
{
    uint8_t alert_id = CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS;
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("is_valid_alert_id(alert_id)", "connectivity_notifier_unregister_notification");

    connectivity_notifier_unregister_notification(alert_id);
}

TEST_ORDERED(ConnectivityNotifier, NotifyAssertsIfAlertIdInvalid, 1)
{
    uint8_t alert_id = CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS;
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("is_valid_alert_id(alert_id)", "connectivity_notifier_notify");

    connectivity_notifier_notify(alert_id, true);
}
