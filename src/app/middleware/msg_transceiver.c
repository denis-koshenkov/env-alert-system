#include "msg_transceiver.h"
#include "hal/transceiver.h"
#include "eas_assert.h"

#define MSG_TRANSCEIVER_MESSAGE_ID_ALERT_STATUS_CHANGE 0
#define MSG_TRANSCEIVER_MESSAGE_ID_REMOVE_ALERT 1
#define MSG_TRANSCEIVER_MESSAGE_ID_ADD_ALERT 2

static MsgTransceiverMessageSentCb message_sent_cb = NULL;
static MsgTransceiverRemoveAlertCb remove_alert_cb = NULL;
static void *remove_alert_cb_user_data = NULL;
static MsgTransceiverAddAlertCb add_alert_cb = NULL;

static void transmit_complete_cb(bool result, void *user_data)
{
    if (message_sent_cb) {
        message_sent_cb(result, user_data);
    }
}

/**
 * @brief Handle receiving a "remove alert" message.
 *
 * @param bytes Received bytes excluding the first message id byte.
 * @param num_bytes Number of bytes in the @p bytes array.
 */
static void handle_remove_alert_message(const uint8_t *const bytes, size_t num_bytes)
{
    EAS_ASSERT(bytes);

    /* There should be exactly one byte in the payload - alert id */
    if (num_bytes != 1) {
        return;
    }

    uint8_t alert_id = bytes[0];
    if (remove_alert_cb) {
        remove_alert_cb(alert_id, remove_alert_cb_user_data);
    }
}

/**
 * @brief Handle receiving a "add alert" message.
 *
 * @param bytes Received bytes excluding the first message id byte.
 * @param num_bytes Number of bytes in the @p bytes array.
 */
static void handle_add_alert_message(const uint8_t *const bytes, size_t num_bytes)
{
    MsgTransceiverAlert alert;
    alert.alert_id = 0;
    alert.warmup_period = 0;
    alert.cooldown_period = 0;
    alert.notification_type.connectivity = 1;
    alert.notification_type.led = 0;
    alert.alert_condition.num_variable_requirements = 1;
    MsgTransceiverVariableRequirement *requirement = &(alert.alert_condition.variable_requirements[0]);
    requirement->variable_identifier = MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_TEMPERATURE;
    requirement->operator = MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_GEQ;
    requirement->constraint_value.temperature = 0;
    requirement->is_last_in_ored_requirement = true;
    if (add_alert_cb) {
        add_alert_cb(&alert, NULL);
    }
}

static void receive_cb(uint8_t *bytes, size_t num_bytes, void *user_data)
{
    EAS_ASSERT(bytes);
    if (num_bytes == 0) {
        return;
    }

    uint8_t message_id = bytes[0];
    switch (message_id) {
    case MSG_TRANSCEIVER_MESSAGE_ID_REMOVE_ALERT:
        handle_remove_alert_message(&bytes[1], num_bytes - 1);
        break;
    case MSG_TRANSCEIVER_MESSAGE_ID_ADD_ALERT:
        handle_add_alert_message(&bytes[1], num_bytes - 1);
        break;
    default:
        break;
    }
}

void msg_transceiver_init()
{
    transceiver_set_receive_cb(receive_cb, NULL);
}

void msg_transceiver_send_alert_status_change_message(uint8_t alert_id, bool is_raised, MsgTransceiverMessageSentCb cb,
                                                      void *user_data)
{
    message_sent_cb = cb;
    uint8_t bytes[3];
    bytes[0] = MSG_TRANSCEIVER_MESSAGE_ID_ALERT_STATUS_CHANGE;
    bytes[1] = alert_id;
    bytes[2] = is_raised ? 0x1 : 0x0;
    /* Pass user_data so that it is available inside transmit_complete_cb, and we can pass it as a parameter to
     * message_sent_cb */
    transceiver_transmit(bytes, 3, transmit_complete_cb, user_data);
}

void msg_transceiver_set_add_alert_cb(MsgTransceiverAddAlertCb cb, void *user_data)
{
    add_alert_cb = cb;
}

void msg_transceiver_set_remove_alert_cb(MsgTransceiverRemoveAlertCb cb, void *user_data)
{
    remove_alert_cb = cb;
    remove_alert_cb_user_data = user_data;
}

void msg_transceiver_deinit()
{
    transceiver_unset_receive_cb();
}
