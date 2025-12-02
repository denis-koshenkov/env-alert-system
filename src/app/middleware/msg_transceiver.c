#include "msg_transceiver.h"
#include "hal/transceiver.h"

#define MSG_TRANSCEIVER_MESSAGE_ID_ALERT_STATUS_CHANGE 0

static MsgTransceiverMessageSentCb message_sent_cb = NULL;

static void transmit_complete_cb(bool result, void *user_data)
{
    message_sent_cb(result, user_data);
}

void msg_transceiver_init()
{
    transceiver_set_receive_cb(NULL, NULL);
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

void msg_transceiver_deinit()
{
    transceiver_unset_receive_cb();
}
