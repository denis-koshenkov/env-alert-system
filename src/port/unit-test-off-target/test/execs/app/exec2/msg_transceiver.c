#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport_c.h"
#include "CppUTestExt/TestAssertPlugin_c.h"

#include "msg_transceiver.h"
#include "hal/transceiver.h"

/* Transceiver mock object populates these every time msg_transceiver implementation calls transceiver_transmit */
static TransceiverTransmitCompleteCb transmit_complete_cb = NULL;
static void *transmit_complete_cb_user_data = NULL;
/* Transceiver mock object populates these every time msg_transceiver implementation calls transceiver_set_receive_cb */
static TransceiverReceiveCb receive_cb = NULL;
static void *receive_cb_user_data = NULL;
/* Populated from inside message_sent_cb */
static bool message_sent_cb_called = false;
static bool message_sent_cb_result = false;
static void *message_sent_cb_user_data = NULL;
/* Populated from inside remove_alert_cb */
static bool remove_alert_cb_called = false;
static uint8_t remove_alert_cb_alert_id = false;
static void *remove_alert_cb_user_data = NULL;

static void message_sent_cb(bool result, void *user_data)
{
    message_sent_cb_called = true;
    message_sent_cb_result = result;
    message_sent_cb_user_data = user_data;
}

static void remove_alert_cb(uint8_t alert_id, void *user_data)
{
    remove_alert_cb_called = true;
    remove_alert_cb_alert_id = alert_id;
    remove_alert_cb_user_data = user_data;
}

TEST_GROUP_C_SETUP(MsgTransceiver)
{
    transmit_complete_cb = NULL;
    transmit_complete_cb_user_data = NULL;
    receive_cb = NULL;
    receive_cb_user_data = NULL;
    message_sent_cb_called = false;
    message_sent_cb_result = false;
    message_sent_cb_user_data = NULL;
    remove_alert_cb_called = false;
    remove_alert_cb_alert_id = 0xFF;
    remove_alert_cb_user_data = NULL;

    mock_c()->strictOrder();

    /* Pass pointers to the mock object so that it populates them TransmitCompleteCb and its user data when
     * transceiver_transmit gets called. We need to execute this callback to trigger MsgTransceiver to execute the
     * MessageSentCb that we pass as a parameter to msg_transceiver_send_alert_status_change_message. */
    mock_c()->setPointerData("transmitCompleteCb", (void **)&transmit_complete_cb);
    mock_c()->setPointerData("transmitCompleteCbUserData", (void **)&transmit_complete_cb_user_data);

    /* Transceiver mock object populates these pointers whenever transceiver_set_receive_cb is called. The test can then
     * simulate receiving bytes by calling this callback. */
    mock_c()->setPointerData("receiveCb", (void **)&receive_cb);
    mock_c()->setPointerData("receiveCbUserData", (void **)&receive_cb_user_data);

    /* msg_transceiver_init */
    mock_c()->expectOneCall("transceiver_set_receive_cb")->ignoreOtherParameters();
    msg_transceiver_init();
}

TEST_GROUP_C_TEARDOWN(MsgTransceiver)
{
    /* msg_transceiver_deinit */
    mock_c()->expectOneCall("transceiver_unset_receive_cb");
    msg_transceiver_deinit();
}

TEST_C(MsgTransceiver, Alert0Silenced)
{
    size_t expected_num_bytes = 3;
    uint8_t expected_payload[] = {0x0, 0x0, 0x0};
    /* msg_transceiver_send_alert_status_change_message */
    mock_c()
        ->expectOneCall("transceiver_transmit")
        ->withMemoryBufferParameter("bytes", expected_payload, expected_num_bytes)
        ->withUnsignedLongIntParameters("num_bytes", expected_num_bytes)
        ->ignoreOtherParameters();

    msg_transceiver_send_alert_status_change_message(0, false, message_sent_cb, NULL);
    /* Mock transmission success */
    transmit_complete_cb(true, transmit_complete_cb_user_data);

    /* message_sent_cb should have been called with result == true */
    CHECK_C(message_sent_cb_called);
    CHECK_C(message_sent_cb_result);
}

TEST_C(MsgTransceiver, Alert0Raised)
{
    size_t expected_num_bytes = 3;
    uint8_t expected_payload[] = {0x0, 0x0, 0x1};
    /* msg_transceiver_send_alert_status_change_message */
    mock_c()
        ->expectOneCall("transceiver_transmit")
        ->withMemoryBufferParameter("bytes", expected_payload, expected_num_bytes)
        ->withUnsignedLongIntParameters("num_bytes", expected_num_bytes)
        ->ignoreOtherParameters();

    msg_transceiver_send_alert_status_change_message(0, true, message_sent_cb, NULL);
    /* Mock transmission success */
    transmit_complete_cb(true, transmit_complete_cb_user_data);

    /* message_sent_cb should have been called with result == true */
    CHECK_C(message_sent_cb_called);
    CHECK_C(message_sent_cb_result);
}

TEST_C(MsgTransceiver, Alert1Silenced)
{
    size_t expected_num_bytes = 3;
    uint8_t expected_payload[] = {0x0, 0x1, 0x0};
    /* msg_transceiver_send_alert_status_change_message */
    mock_c()
        ->expectOneCall("transceiver_transmit")
        ->withMemoryBufferParameter("bytes", expected_payload, expected_num_bytes)
        ->withUnsignedLongIntParameters("num_bytes", expected_num_bytes)
        ->ignoreOtherParameters();

    msg_transceiver_send_alert_status_change_message(1, false, message_sent_cb, NULL);
    /* Mock transmission success */
    transmit_complete_cb(true, transmit_complete_cb_user_data);

    /* message_sent_cb should have been called with result == true */
    CHECK_C(message_sent_cb_called);
    CHECK_C(message_sent_cb_result);
}

TEST_C(MsgTransceiver, Alert8RaisedFailed)
{
    size_t expected_num_bytes = 3;
    uint8_t expected_payload[] = {0x0, 0x8, 0x1};
    /* msg_transceiver_send_alert_status_change_message */
    mock_c()
        ->expectOneCall("transceiver_transmit")
        ->withMemoryBufferParameter("bytes", expected_payload, expected_num_bytes)
        ->withUnsignedLongIntParameters("num_bytes", expected_num_bytes)
        ->ignoreOtherParameters();

    msg_transceiver_send_alert_status_change_message(8, true, message_sent_cb, NULL);
    /* Mock transmission failure */
    transmit_complete_cb(false, transmit_complete_cb_user_data);

    /* message_sent_cb should have been called with result == false */
    CHECK_C(message_sent_cb_called);
    CHECK_C(!message_sent_cb_result);
}

TEST_C(MsgTransceiver, MessageSentCbCalledWithUserData)
{
    void *user_data = (void *)0x42;
    size_t expected_num_bytes = 3;
    uint8_t expected_payload[] = {0x0, 20, 0x0};
    /* msg_transceiver_send_alert_status_change_message */
    mock_c()
        ->expectOneCall("transceiver_transmit")
        ->withMemoryBufferParameter("bytes", expected_payload, expected_num_bytes)
        ->withUnsignedLongIntParameters("num_bytes", expected_num_bytes)
        ->ignoreOtherParameters();

    msg_transceiver_send_alert_status_change_message(20, false, message_sent_cb, user_data);
    /* Mock transmission success */
    transmit_complete_cb(true, transmit_complete_cb_user_data);

    CHECK_C(message_sent_cb_called);
    CHECK_C(message_sent_cb_result);
    CHECK_EQUAL_C_POINTER(user_data, message_sent_cb_user_data);
}

TEST_C(MsgTransceiver, RemoveAlert0)
{
    msg_transceiver_set_remove_alert_cb(remove_alert_cb, NULL);
    /* Mock receiving a "remove alert" message. 0x1 - message id, 0 - alert id */
    uint8_t remove_alert_bytes[2] = {0x1, 0};
    receive_cb(remove_alert_bytes, 2, receive_cb_user_data);

    /* remove_alert_cb should have been called with alert_id 0 and user_data NULL */
    CHECK_C(remove_alert_cb_called);
    CHECK_EQUAL_C_UINT(0, remove_alert_cb_alert_id);
    CHECK_EQUAL_C_POINTER(NULL, remove_alert_cb_user_data);
}

TEST_C(MsgTransceiver, RemoveAlert1)
{
    msg_transceiver_set_remove_alert_cb(remove_alert_cb, NULL);
    /* Mock receiving a "remove alert" message. 0x1 - message id, 1 - alert id */
    uint8_t remove_alert_bytes[2] = {0x1, 1};
    receive_cb(remove_alert_bytes, 2, receive_cb_user_data);

    /* remove_alert_cb should have been called with alert_id 1 and user_data NULL */
    CHECK_C(remove_alert_cb_called);
    CHECK_EQUAL_C_UINT(1, remove_alert_cb_alert_id);
    CHECK_EQUAL_C_POINTER(NULL, remove_alert_cb_user_data);
}

TEST_C(MsgTransceiver, ReceiveCbCalledWithNumBytes0)
{
    msg_transceiver_set_remove_alert_cb(remove_alert_cb, NULL);
    /* Not giving a NULL pointer as receive_bytes, because there is a separate check for that - we want to specifically
     * test the check for number of bytes in this test. */
    uint8_t receive_bytes[1] = {0xAA};
    receive_cb(receive_bytes, 0, receive_cb_user_data);

    /* No callbacks should be called as a result of receiving 0 bytes. Also checking that that the program does not
     * crash. */
    CHECK_C(!remove_alert_cb_called);
}

TEST_C(MsgTransceiver, RemoveAlertMessageWithNoAlertId)
{
    msg_transceiver_set_remove_alert_cb(remove_alert_cb, NULL);
    /* 0x1 is the message id for "remove alert" message. The payload should also contain the second byte - alert id, but
     * it does not. */
    uint8_t receive_bytes[1] = {0x1};
    receive_cb(receive_bytes, 1, receive_cb_user_data);

    /* No callbacks should be called - message payload structure is invalid. */
    CHECK_C(!remove_alert_cb_called);
}

TEST_C(MsgTransceiver, RemoveAlertMessageTooManyBytes)
{
    msg_transceiver_set_remove_alert_cb(remove_alert_cb, NULL);
    /* "Remove alert" message should have only two bytes - message id and alert id. Here it has three bytes, so message
     * should be ignored. */
    uint8_t receive_bytes[3] = {0x1, 5, 0xFF};
    receive_cb(receive_bytes, 3, receive_cb_user_data);

    /* No callbacks should be called - message payload structure is invalid. */
    CHECK_C(!remove_alert_cb_called);
}

TEST_C(MsgTransceiver, ReceiveCbBytesNullPointerAssert)
{
    TEST_ASSERT_PLUGIN_C_EXPECT_ASSERTION("bytes", "receive_cb");

    msg_transceiver_set_remove_alert_cb(remove_alert_cb, NULL);
    /* Not giving 0 as num_bytes, because there is a separate check for that - we want to specifically test the NULL
     * pointer check in this test. */
    receive_cb(NULL, 5, receive_cb_user_data);
}

TEST_C(MsgTransceiver, RemoveAlertCbExecutedWithUserData)
{
    void *user_data = (void *)0xAB;
    msg_transceiver_set_remove_alert_cb(remove_alert_cb, user_data);
    /* Mock receiving a "remove alert" message. 0x1 - message id, 10 - alert id */
    uint8_t remove_alert_bytes[2] = {0x1, 10};
    receive_cb(remove_alert_bytes, 2, receive_cb_user_data);

    /* remove_alert_cb should have been called with alert_id 0 and user_data NULL */
    CHECK_C(remove_alert_cb_called);
    CHECK_EQUAL_C_UINT(10, remove_alert_cb_alert_id);
    CHECK_EQUAL_C_POINTER(user_data, remove_alert_cb_user_data);
}
