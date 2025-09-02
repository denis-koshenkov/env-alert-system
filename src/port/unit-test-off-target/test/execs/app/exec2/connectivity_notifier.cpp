#include "CppUTest/TestHarness.h"
#include "CppUTestExt/OrderedTest.h"

#include "connectivity_notifier.h"
#include "config.h"

TEST_GROUP(ConnectivityNotifier){};

TEST_ORDERED(ConnectivityNotifier, AllNotificationsInitiallyUnregistered, 0)
{
    /* Do not expect any calls to connectivity_notification_sender */

    for (size_t alert_id = 0; alert_id < CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS; alert_id++) {
        connectivity_notifier_notify(alert_id, true);
    }
}
