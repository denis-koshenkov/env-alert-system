#include "msg_transceiver.h"
#include "hal/transceiver.h"

static MsgTransceiverMessageSentCb message_sent_cb = NULL;
static void *message_sent_cb_user_data = NULL;

static void transmit_complete_cb(bool result, void *user_data)
{
    message_sent_cb(true, message_sent_cb_user_data);
}

void msg_transceiver_init()
{
    transceiver_set_receive_cb(NULL, NULL);
}

void msg_transceiver_send_alert_status_change_message(uint8_t alert_id, bool is_raised, MsgTransceiverMessageSentCb cb,
                                                      void *user_data)
{
    message_sent_cb = cb;
    message_sent_cb_user_data = user_data;
    uint8_t bytes[] = {0x0, 0x0, 0x0};
    transceiver_transmit(bytes, 3, transmit_complete_cb, NULL);
}

void msg_transceiver_deinit()
{
    transceiver_unset_receive_cb();
}
