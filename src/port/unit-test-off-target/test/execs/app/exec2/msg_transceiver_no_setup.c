#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/TestAssertPlugin_c.h"

#include "msg_transceiver.h"

static void message_sent_cb(bool result, void *user_data)
{
}

TEST_C(MsgTransceiverNoSetup, SendAlertStatusChangeMessageCalledBeforeInit)
{
    TEST_ASSERT_PLUGIN_C_EXPECT_ASSERTION("initialized", "msg_transceiver_send_alert_status_change_message");
    msg_transceiver_send_alert_status_change_message(0, false, message_sent_cb, NULL);
}
