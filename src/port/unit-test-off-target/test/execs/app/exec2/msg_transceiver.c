#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport_c.h"

#include "msg_transceiver.h"
#include "hal/transceiver.h"

static TransceiverTransmitCompleteCb transmit_complete_cb = NULL;
static void *transmit_complete_cb_user_data = NULL;
static bool message_sent_cb_called = false;
static bool message_sent_cb_result = false;
static void *message_sent_cb_user_data = NULL;

static void message_sent_cb(bool result, void *user_data)
{
    message_sent_cb_called = true;
    message_sent_cb_result = result;
    message_sent_cb_user_data = user_data;
}

TEST_GROUP_C_SETUP(MsgTransceiver)
{
    transmit_complete_cb = NULL;
    transmit_complete_cb_user_data = NULL;
    message_sent_cb_called = false;
    message_sent_cb_result = false;
    message_sent_cb_user_data = NULL;

    mock_c()->strictOrder();

    /* Pass pointers to the mock object so that it populates them TransmitCompleteCb and its user data when
     * transceiver_transmit gets called. We need to execute this callback to trigger MsgTransceiver to execute the
     * MessageSentCb that we pass as a parameter to msg_transceiver_send_alert_status_change_message. */
    mock_c()->setPointerData("transmitCompleteCb", (void **)&transmit_complete_cb);
    mock_c()->setPointerData("transmitCompleteCbUserData", (void **)&transmit_complete_cb_user_data);
}

TEST_C(MsgTransceiver, Alert0Silenced)
{
    /* msg_transceiver_init */
    mock_c()->expectOneCall("transceiver_set_receive_cb")->ignoreOtherParameters();
    size_t expected_num_bytes = 3;
    uint8_t expected_payload[] = {0x0, 0x0, 0x0};
    /* msg_transceiver_send_alert_status_change_message */
    mock_c()
        ->expectOneCall("transceiver_transmit")
        ->withMemoryBufferParameter("bytes", expected_payload, expected_num_bytes)
        ->withUnsignedLongIntParameters("num_bytes", expected_num_bytes)
        ->ignoreOtherParameters();
    /* msg_transceiver_deinit */
    mock_c()->expectOneCall("transceiver_unset_receive_cb");

    msg_transceiver_init();
    msg_transceiver_send_alert_status_change_message(0, false, message_sent_cb, NULL);
    /* Mock transmission success */
    transmit_complete_cb(true, transmit_complete_cb_user_data);

    /* message_sent_cb should have been called with result == true */
    CHECK_C(message_sent_cb_called);
    CHECK_C(message_sent_cb_result);

    msg_transceiver_deinit();
}

TEST_C(MsgTransceiver, Alert0Raised)
{
    /* msg_transceiver_init */
    mock_c()->expectOneCall("transceiver_set_receive_cb")->ignoreOtherParameters();
    size_t expected_num_bytes = 3;
    uint8_t expected_payload[] = {0x0, 0x0, 0x1};
    /* msg_transceiver_send_alert_status_change_message */
    mock_c()
        ->expectOneCall("transceiver_transmit")
        ->withMemoryBufferParameter("bytes", expected_payload, expected_num_bytes)
        ->withUnsignedLongIntParameters("num_bytes", expected_num_bytes)
        ->ignoreOtherParameters();
    /* msg_transceiver_deinit */
    mock_c()->expectOneCall("transceiver_unset_receive_cb");

    msg_transceiver_init();
    msg_transceiver_send_alert_status_change_message(0, true, message_sent_cb, NULL);
    /* Mock transmission success */
    transmit_complete_cb(true, transmit_complete_cb_user_data);

    /* message_sent_cb should have been called with result == true */
    CHECK_C(message_sent_cb_called);
    CHECK_C(message_sent_cb_result);

    msg_transceiver_deinit();
}

TEST_C(MsgTransceiver, Alert1Silenced)
{
    /* msg_transceiver_init */
    mock_c()->expectOneCall("transceiver_set_receive_cb")->ignoreOtherParameters();
    size_t expected_num_bytes = 3;
    uint8_t expected_payload[] = {0x0, 0x1, 0x0};
    /* msg_transceiver_send_alert_status_change_message */
    mock_c()
        ->expectOneCall("transceiver_transmit")
        ->withMemoryBufferParameter("bytes", expected_payload, expected_num_bytes)
        ->withUnsignedLongIntParameters("num_bytes", expected_num_bytes)
        ->ignoreOtherParameters();
    /* msg_transceiver_deinit */
    mock_c()->expectOneCall("transceiver_unset_receive_cb");

    msg_transceiver_init();
    msg_transceiver_send_alert_status_change_message(1, false, message_sent_cb, NULL);
    /* Mock transmission success */
    transmit_complete_cb(true, transmit_complete_cb_user_data);

    /* message_sent_cb should have been called with result == true */
    CHECK_C(message_sent_cb_called);
    CHECK_C(message_sent_cb_result);

    msg_transceiver_deinit();
}

TEST_C(MsgTransceiver, Alert8RaisedFailed)
{
    /* msg_transceiver_init */
    mock_c()->expectOneCall("transceiver_set_receive_cb")->ignoreOtherParameters();
    size_t expected_num_bytes = 3;
    uint8_t expected_payload[] = {0x0, 0x8, 0x1};
    /* msg_transceiver_send_alert_status_change_message */
    mock_c()
        ->expectOneCall("transceiver_transmit")
        ->withMemoryBufferParameter("bytes", expected_payload, expected_num_bytes)
        ->withUnsignedLongIntParameters("num_bytes", expected_num_bytes)
        ->ignoreOtherParameters();
    /* msg_transceiver_deinit */
    mock_c()->expectOneCall("transceiver_unset_receive_cb");

    msg_transceiver_init();
    msg_transceiver_send_alert_status_change_message(8, true, message_sent_cb, NULL);
    /* Mock transmission failure */
    transmit_complete_cb(false, transmit_complete_cb_user_data);

    /* message_sent_cb should have been called with result == false */
    CHECK_C(message_sent_cb_called);
    CHECK_C(!message_sent_cb_result);

    msg_transceiver_deinit();
}

TEST_C(MsgTransceiver, MessageSentCbCalledWithUserData)
{
    void *user_data = (void *)0x42;

    /* msg_transceiver_init */
    mock_c()->expectOneCall("transceiver_set_receive_cb")->ignoreOtherParameters();
    size_t expected_num_bytes = 3;
    uint8_t expected_payload[] = {0x0, 20, 0x0};
    /* msg_transceiver_send_alert_status_change_message */
    mock_c()
        ->expectOneCall("transceiver_transmit")
        ->withMemoryBufferParameter("bytes", expected_payload, expected_num_bytes)
        ->withUnsignedLongIntParameters("num_bytes", expected_num_bytes)
        ->ignoreOtherParameters();
    /* msg_transceiver_deinit */
    mock_c()->expectOneCall("transceiver_unset_receive_cb");

    msg_transceiver_init();
    msg_transceiver_send_alert_status_change_message(20, false, message_sent_cb, user_data);
    /* Mock transmission success */
    transmit_complete_cb(true, transmit_complete_cb_user_data);

    CHECK_C(message_sent_cb_called);
    CHECK_C(message_sent_cb_result);
    CHECK_EQUAL_C_POINTER(user_data, message_sent_cb_user_data);

    msg_transceiver_deinit();
}
