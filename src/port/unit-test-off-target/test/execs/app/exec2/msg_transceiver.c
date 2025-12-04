#include <string.h>

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
/* Populated from inside add_alert_cb */
static bool add_alert_cb_called = false;
static MsgTransceiverAlert add_alert_cb_alert;
static void *add_alert_cb_user_data = NULL;

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

static void add_alert_cb(const MsgTransceiverAlert *const alert, void *user_data)
{
    add_alert_cb_called = true;
    memcpy(&add_alert_cb_alert, alert, sizeof(MsgTransceiverAlert));
    add_alert_cb_user_data = user_data;
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
    add_alert_cb_called = false;
    /* Some tests test for 0 values, so the tests are more reliable if the alert memory is initially set to 0xFF*/
    memset(&add_alert_cb_alert, 0xFF, sizeof(MsgTransceiverAlert));
    add_alert_cb_user_data = NULL;

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

TEST_C(MsgTransceiver, AddAlert0)
{
    msg_transceiver_set_add_alert_cb(add_alert_cb, NULL);
    /* Mock receiving a "add alert" message */
    uint8_t add_alert_bytes[17] = {
        0x2,                /* message id */
        0x0,                /* alert id */
        0x0, 0x0, 0x0, 0x0, /* Warmup period - 0 ms */
        0x0, 0x0, 0x0, 0x0, /* Cooldown period - 0 ms */
        0x1,                /* notification type - connectivity enabled, LED disabled */
        0x1,                /* Number of ORed requirements */
        0x1,                /* Number of variable requirements in the first ORed requirement */
        /* Start of variable requirement 0 */
        0x0,     /* Temperature variable identifier */
        0x0,     /* Operator - greater than or equal to */
        0x0, 0x0 /* Constraint value - 0 degrees Celsius */
    };
    receive_cb(add_alert_bytes, 17, receive_cb_user_data);

    CHECK_C(add_alert_cb_called);
    /* Validate constructed alert */
    const MsgTransceiverAlert *const alert = &add_alert_cb_alert;
    CHECK_EQUAL_C_UBYTE(0, alert->alert_id);
    CHECK_EQUAL_C_ULONG(0, alert->warmup_period);
    CHECK_EQUAL_C_ULONG(0, alert->cooldown_period);
    CHECK_C(alert->notification_type.connectivity);
    CHECK_C(!(alert->notification_type.led));
    CHECK_EQUAL_C_UBYTE(1, alert->alert_condition.num_variable_requirements);
    const MsgTransceiverVariableRequirement *requirement = &(alert->alert_condition.variable_requirements[0]);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_TEMPERATURE, requirement->variable_identifier);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_GEQ, requirement->operator);
    CHECK_EQUAL_C_LONG(0, requirement->constraint_value.temperature);
    CHECK_C(requirement->is_last_in_ored_requirement);
}

TEST_C(MsgTransceiver, AddAlert1)
{
    msg_transceiver_set_add_alert_cb(add_alert_cb, NULL);
    /* Mock receiving a "add alert" message */
    uint8_t add_alert_bytes[19] = {
        0x2,                 /* message id */
        0x1,                 /* alert id */
        0xE8, 0x3, 0x0, 0x0, /* Warmup period - 1000 ms */
        0xD0, 0x7, 0x0, 0x0, /* Cooldown period - 2000 ms */
        0x2,                 /* notification type - connectivity disabled, LED enabled */
        0x0,                 /* Led color - red */
        0x0,                 /* Led pattern - static */
        0x1,                 /* Number of ORed requirements */
        0x1,                 /* Number of variable requirements in the first ORed requirement */
        /* Start of variable requirement 0 */
        0x1,      /* Pressure variable identifier */
        0x1,      /* Operator - less than or equal to */
        0xE7, 0x3 /* Constraint value - 999 hPa */
    };
    receive_cb(add_alert_bytes, 17, receive_cb_user_data);

    CHECK_C(add_alert_cb_called);
    /* Validate constructed alert */
    const MsgTransceiverAlert *const alert = &add_alert_cb_alert;
    CHECK_EQUAL_C_UBYTE(1, alert->alert_id);
    CHECK_EQUAL_C_ULONG(1000, alert->warmup_period);
    CHECK_EQUAL_C_ULONG(2000, alert->cooldown_period);
    CHECK_C(!(alert->notification_type.connectivity));
    CHECK_C(alert->notification_type.led);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_LED_COLOR_RED, alert->led_color);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_LED_PATTERN_STATIC, alert->led_pattern);
    CHECK_EQUAL_C_UBYTE(1, alert->alert_condition.num_variable_requirements);
    const MsgTransceiverVariableRequirement *requirement = &(alert->alert_condition.variable_requirements[0]);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_PRESSURE, requirement->variable_identifier);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_LEQ, requirement->operator);
    CHECK_EQUAL_C_ULONG(999, requirement->constraint_value.pressure);
    CHECK_C(requirement->is_last_in_ored_requirement);
}

TEST_C(MsgTransceiver, AddAlert2)
{
    msg_transceiver_set_add_alert_cb(add_alert_cb, NULL);
    /* Mock receiving a "add alert" message */
    uint8_t add_alert_bytes[25] = {
        0x2,                  /* message id */
        0x2,                  /* alert id */
        0xC0, 0xD4, 0x1, 0x0, /* Warmup period - 120,000 ms */
        0xB3, 0x15, 0x0, 0x0, /* Cooldown period - 5555 ms */
        0x3,                  /* notification type - connectivity enabled, LED enabled */
        0x1,                  /* Led color - green */
        0x1,                  /* Led pattern - alert */
        0x1,                  /* Number of ORed requirements */
        0x2,                  /* Number of variable requirements in the first ORed requirement */
        /* Start of variable requirement 0 */
        0x2,       /* Humidity variable identifier */
        0x0,       /* Operator - greater than or equal to */
        0xE8, 0x3, /* Constraint value 1000 -> 100.0% RH */
        /* Start of variable requirement 1 */
        0x3,               /* Light intensity variable identifier */
        0x1,               /* Operator - less than or equal to */
        0x1, 0x0, 0x0, 0x0 /* Constraint value 1 lx */
    };
    receive_cb(add_alert_bytes, 17, receive_cb_user_data);

    CHECK_C(add_alert_cb_called);
    /* Validate constructed alert */
    const MsgTransceiverAlert *const alert = &add_alert_cb_alert;
    CHECK_EQUAL_C_UBYTE(2, alert->alert_id);
    CHECK_EQUAL_C_ULONG(120000, alert->warmup_period);
    CHECK_EQUAL_C_ULONG(5555, alert->cooldown_period);
    CHECK_C(alert->notification_type.connectivity);
    CHECK_C(alert->notification_type.led);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_LED_COLOR_GREEN, alert->led_color);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_LED_PATTERN_ALERT, alert->led_pattern);
    CHECK_EQUAL_C_UBYTE(2, alert->alert_condition.num_variable_requirements);
    const MsgTransceiverVariableRequirement *requirement0 = &(alert->alert_condition.variable_requirements[0]);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_HUMIDITY, requirement0->variable_identifier);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_GEQ, requirement0->operator);
    CHECK_EQUAL_C_ULONG(1000, requirement0->constraint_value.humidity);
    CHECK_C(!requirement0->is_last_in_ored_requirement);
    const MsgTransceiverVariableRequirement *requirement1 = &(alert->alert_condition.variable_requirements[1]);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_LIGHT_INTENSITY, requirement1->variable_identifier);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_LEQ, requirement1->operator);
    CHECK_EQUAL_C_ULONG(1, requirement1->constraint_value.light_intensity);
    CHECK_C(requirement1->is_last_in_ored_requirement);
}

TEST_C(MsgTransceiver, AddAlert3)
{
    msg_transceiver_set_add_alert_cb(add_alert_cb, NULL);
    /* Mock receiving a "add alert" message */
    uint8_t add_alert_bytes[36] = {
        0x2,                 /* message id */
        0x3,                 /* alert id */
        0xA, 0x0, 0x0, 0x0,  /* Warmup period - 10 ms */
        0x14, 0x0, 0x0, 0x0, /* Cooldown period - 20 ms */
        0x0,                 /* notification type - connectivity disabled, LED disabled */
        0x2,                 /* Number of ORed requirements */

        /* Start of first ORed requirement */
        0x2, /* Number of variable requirements in the first ORed requirement */

        /* Start of variable requirement 0 */
        0x0,       /* Temperature variable identifier */
        0x0,       /* Operator - greater than or equal to */
        0xCD, 0x0, /* Constraint value 205 -> 20.5 degrees Celsius */
        /* Start of variable requirement 1 */
        0x1,        /* Pressure variable identifier */
        0x0,        /* Operator - greater than or equal to */
        0x67, 0x2B, /* Constraint value 11111 -> 1111.1 hPa */

        /* Start of second ORed requirement */
        0x3, /* Number of variable requirements in the second ORed requirement */

        /* Start of variable requirement 0 */
        0x3,                  /* Light intensity variable identifier */
        0x1,                  /* Operator - less than or equal to */
        0xA0, 0x86, 0x1, 0x0, /* Constraint value 100000 -> 100,000 lx */
        /* Start of variable requirement 1 */
        0x2,       /* Humidity variable identifier */
        0x1,       /* Operator - less than or equal to */
        0xA4, 0x1, /* Constraint value 420 -> 42.0 % RH */
        /* Start of variable requirement 2 */
        0x0,        /* Temperature variable identifier */
        0x1,        /* Operator - less than or equal to */
        0xD3, 0xFE, /* Constraint value -301 -> -30.1 degrees Celsius */

    };
    receive_cb(add_alert_bytes, 17, receive_cb_user_data);

    CHECK_C(add_alert_cb_called);
    /* Validate constructed alert */
    const MsgTransceiverAlert *const alert = &add_alert_cb_alert;
    CHECK_EQUAL_C_UBYTE(3, alert->alert_id);
    CHECK_EQUAL_C_ULONG(10, alert->warmup_period);
    CHECK_EQUAL_C_ULONG(20, alert->cooldown_period);
    CHECK_C(!(alert->notification_type.connectivity));
    CHECK_C(!(alert->notification_type.led));
    CHECK_EQUAL_C_UBYTE(5, alert->alert_condition.num_variable_requirements);
    const MsgTransceiverVariableRequirement *requirement0 = &(alert->alert_condition.variable_requirements[0]);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_TEMPERATURE, requirement0->variable_identifier);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_GEQ, requirement0->operator);
    CHECK_EQUAL_C_ULONG(205, requirement0->constraint_value.temperature);
    CHECK_C(!requirement0->is_last_in_ored_requirement);
    const MsgTransceiverVariableRequirement *requirement1 = &(alert->alert_condition.variable_requirements[1]);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_PRESSURE, requirement1->variable_identifier);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_GEQ, requirement1->operator);
    CHECK_EQUAL_C_ULONG(11111, requirement1->constraint_value.pressure);
    CHECK_C(requirement1->is_last_in_ored_requirement);
    const MsgTransceiverVariableRequirement *requirement2 = &(alert->alert_condition.variable_requirements[2]);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_LIGHT_INTENSITY, requirement2->variable_identifier);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_LEQ, requirement2->operator);
    CHECK_EQUAL_C_ULONG(100000, requirement2->constraint_value.light_intensity);
    CHECK_C(!requirement2->is_last_in_ored_requirement);
    const MsgTransceiverVariableRequirement *requirement3 = &(alert->alert_condition.variable_requirements[3]);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_HUMIDITY, requirement3->variable_identifier);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_LEQ, requirement3->operator);
    CHECK_EQUAL_C_ULONG(420, requirement3->constraint_value.humidity);
    CHECK_C(!requirement3->is_last_in_ored_requirement);
    const MsgTransceiverVariableRequirement *requirement4 = &(alert->alert_condition.variable_requirements[4]);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_TEMPERATURE, requirement4->variable_identifier);
    CHECK_EQUAL_C_UBYTE(MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_LEQ, requirement4->operator);
    CHECK_EQUAL_C_ULONG(-301, requirement4->constraint_value.temperature);
    CHECK_C(requirement4->is_last_in_ored_requirement);
}

TEST_C(MsgTransceiver, InvalidMessageId)
{
    msg_transceiver_set_add_alert_cb(add_alert_cb, NULL);
    msg_transceiver_set_remove_alert_cb(remove_alert_cb, NULL);

    /* Mock receiving a "add alert" message */
    uint8_t bytes[5] = {
        0xA0,                  /* invalid message id */
        0x1,  0x4, 0xFF, 0xA5, /* Some garbage data */
    };
    receive_cb(bytes, 5, receive_cb_user_data);

    CHECK_C(!add_alert_cb_called);
    CHECK_C(!remove_alert_cb_called);
}

TEST_C(MsgTransceiver, AddAlertMessageOnlyMessageId)
{
    msg_transceiver_set_add_alert_cb(add_alert_cb, NULL);
    msg_transceiver_set_remove_alert_cb(remove_alert_cb, NULL);

    uint8_t bytes[1] = {
        0x2, /* message id */
    };
    receive_cb(bytes, 1, receive_cb_user_data);

    CHECK_C(!add_alert_cb_called);
    CHECK_C(!remove_alert_cb_called);
}

TEST_C(MsgTransceiver, AddAlertMessage2ValidBytes)
{
    msg_transceiver_set_add_alert_cb(add_alert_cb, NULL);
    msg_transceiver_set_remove_alert_cb(remove_alert_cb, NULL);

    uint8_t bytes[2] = {
        0x2, /* message id */
        0x1, /* alert id */
    };
    receive_cb(bytes, 2, receive_cb_user_data);

    CHECK_C(!add_alert_cb_called);
    CHECK_C(!remove_alert_cb_called);
}

TEST_C(MsgTransceiver, AddAlertMessage3ValidBytes)
{
    msg_transceiver_set_add_alert_cb(add_alert_cb, NULL);
    msg_transceiver_set_remove_alert_cb(remove_alert_cb, NULL);

    uint8_t bytes[3] = {
        0x2, /* message id */
        0x2, /* alert id */
        0x1  /* First byte of warmup period */
    };
    receive_cb(bytes, 3, receive_cb_user_data);

    CHECK_C(!add_alert_cb_called);
    CHECK_C(!remove_alert_cb_called);
}

TEST_C(MsgTransceiver, AddAlertMessage4ValidBytes)
{
    msg_transceiver_set_add_alert_cb(add_alert_cb, NULL);
    msg_transceiver_set_remove_alert_cb(remove_alert_cb, NULL);

    uint8_t bytes[4] = {
        0x2,     /* message id */
        0x2,     /* alert id */
        0x1, 0x5 /* First two bytes of warmup period */
    };
    receive_cb(bytes, 4, receive_cb_user_data);

    CHECK_C(!add_alert_cb_called);
    CHECK_C(!remove_alert_cb_called);
}

TEST_C(MsgTransceiver, AddAlertMessage5ValidBytes)
{
    msg_transceiver_set_add_alert_cb(add_alert_cb, NULL);
    msg_transceiver_set_remove_alert_cb(remove_alert_cb, NULL);

    uint8_t bytes[5] = {
        0x2,          /* message id */
        0x2,          /* alert id */
        0x1, 0x5, 0x0 /* First three bytes of warmup period */
    };
    receive_cb(bytes, 5, receive_cb_user_data);

    CHECK_C(!add_alert_cb_called);
    CHECK_C(!remove_alert_cb_called);
}

TEST_C(MsgTransceiver, AddAlertMessage6ValidBytes)
{
    msg_transceiver_set_add_alert_cb(add_alert_cb, NULL);
    msg_transceiver_set_remove_alert_cb(remove_alert_cb, NULL);

    uint8_t bytes[6] = {
        0x2,               /* message id */
        0x2,               /* alert id */
        0x1, 0x5, 0x0, 0x0 /* warmup period */
    };
    receive_cb(bytes, 6, receive_cb_user_data);

    CHECK_C(!add_alert_cb_called);
    CHECK_C(!remove_alert_cb_called);
}
