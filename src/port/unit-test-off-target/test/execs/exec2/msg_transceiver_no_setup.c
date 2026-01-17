#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/TestAssertPlugin_c.h"
#include "CppUTestExt/MockSupport_c.h"

#include "msg_transceiver.h"
#include "transceiver.h"

/* Transceiver mock object populates these every time msg_transceiver implementation calls transceiver_set_receive_cb */
static TransceiverReceiveCb receive_cb = NULL;
static void *receive_cb_user_data = NULL;
/* The test should assign this variable to true in order for msg_transceiver_deinit to be called in test teardown */
static bool is_msg_transceiver_initialized = false;

static void message_sent_cb(bool result, void *user_data)
{
}

static void add_alert_cb(const MsgTransceiverAlert *const alert, void *user_data)
{
}

static void remove_alert_cb(uint8_t alert_id, void *user_data)
{
}

TEST_GROUP_C_SETUP(MsgTransceiverNoSetup)
{
    /* Transceiver mock object populates these pointers whenever transceiver_set_receive_cb is called. The test can then
     * simulate receiving bytes by calling this callback. */
    mock_c()->setPointerData("receiveCb", (void **)&receive_cb);
    mock_c()->setPointerData("receiveCbUserData", (void **)&receive_cb_user_data);
}

TEST_GROUP_C_TEARDOWN(MsgTransceiverNoSetup)
{
    if (is_msg_transceiver_initialized) {
        /* Expected to be called in msg_transceiver_deinit */
        mock_c()->expectOneCall("transceiver_unset_receive_cb");

        /* This has to be here because of the InitCalledTwice test. The test terminates during the second call to init,
         but msg_transceiver is then initialized at the end of the test. At the end of each test, it should be
         in the deinitialized state. */
        msg_transceiver_deinit();
        is_msg_transceiver_initialized = false;
    }
}

TEST_C(MsgTransceiverNoSetup, SendAlertStatusChangeMessageCalledBeforeInit)
{
    TEST_ASSERT_PLUGIN_C_EXPECT_ASSERTION("initialized", "msg_transceiver_send_alert_status_change_message");
    msg_transceiver_send_alert_status_change_message(0, false, message_sent_cb, NULL);
}

TEST_C(MsgTransceiverNoSetup, SetAddAlertCbCalledBeforeInit)
{
    TEST_ASSERT_PLUGIN_C_EXPECT_ASSERTION("initialized", "msg_transceiver_set_add_alert_cb");
    msg_transceiver_set_add_alert_cb(add_alert_cb, NULL);
}

TEST_C(MsgTransceiverNoSetup, SetRemoveAlertCbCalledBeforeInit)
{
    TEST_ASSERT_PLUGIN_C_EXPECT_ASSERTION("initialized", "msg_transceiver_set_remove_alert_cb");
    msg_transceiver_set_remove_alert_cb(remove_alert_cb, NULL);
}

TEST_C(MsgTransceiverNoSetup, InitCalledTwice)
{
    /* Expected to be called in the first call to msg_transceiver_init */
    mock_c()->expectOneCall("transceiver_set_receive_cb")->ignoreOtherParameters();
    TEST_ASSERT_PLUGIN_C_EXPECT_ASSERTION("!initialized", "msg_transceiver_init");

    msg_transceiver_init();
    is_msg_transceiver_initialized = true;
    msg_transceiver_init();
}

TEST_C(MsgTransceiverNoSetup, DeinitDoesNothingIfAlreadyDeinitialized)
{
    /* Expected to be called in first call to msg_transceiver_init */
    mock_c()->expectOneCall("transceiver_set_receive_cb")->ignoreOtherParameters();
    /* Expected to be called in the first call to msg_transceiver_deinit */
    mock_c()->expectOneCall("transceiver_unset_receive_cb");
    /* Expected to be called in second call to msg_transceiver_init */
    mock_c()->expectOneCall("transceiver_set_receive_cb")->ignoreOtherParameters();

    msg_transceiver_init();
    msg_transceiver_deinit();
    msg_transceiver_deinit();
    /* msg_transceiver_init does not assert when it is called after deinit is called several times */
    msg_transceiver_init();
    /* So that msg_transceiver_deinit is called in the teardown to bring system to initial state */
    is_msg_transceiver_initialized = true;
}
