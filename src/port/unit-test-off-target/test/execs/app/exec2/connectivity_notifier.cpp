#include "CppUTest/TestHarness.h"
#include "CppUTestExt/OrderedTest.h"
#include "CppUTestExt/MockSupport.h"

#include "connectivity_notifier.h"
#include "config.h"

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

TEST_ORDERED(ConnectivityNotifier, RegisteredNotificationCallsSender, 1)
{
    uint8_t alert_id = 0;

    mock()
        .expectOneCall("connectivity_notification_sender_send")
        .withParameter("alert_id", alert_id)
        .withParameter("is_raised", true);

    connectivity_notifier_register_notification(alert_id);
    /* This calls connectivity_notification_sender_send, since the notification is now registered */
    connectivity_notifier_notify(alert_id, true);
}
