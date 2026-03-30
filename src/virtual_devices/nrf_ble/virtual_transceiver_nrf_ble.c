#include "virtual_transceiver_nrf_ble.h"
#include "eass.h"
#include "transmit_cb_data_allocator.h"
#include "eas_log.h"
#include "eas_assert.h"

EAS_LOG_ENABLE_IN_FILE();

/* Macro for readability */
#define NOTIFY_SUCCESS true

static TransceiverReceiveCb receive_cb = NULL;
static void *receive_cb_user_data = NULL;

static void transceiver_transmit(const uint8_t *const bytes, size_t num_bytes, TransceiverTransmitCompleteCb cb,
                                 void *user_data);
static void transceiver_set_receive_cb(TransceiverReceiveCb cb, void *user_data);
static void transceiver_unset_receive_cb();

static Transceiver transceiver = {
    .transmit = transceiver_transmit,
    .set_receive_cb = transceiver_set_receive_cb,
    .unset_receive_cb = transceiver_unset_receive_cb,
};

static void eass_received_cb(const uint8_t *data, uint16_t len)
{
    if (receive_cb) {
        receive_cb(data, len, receive_cb_user_data);
    }
}

static void notify_complete_cb(struct bt_conn *conn, void *user_data)
{
    NrfBleTransceiverTransmitCbData *data = (NrfBleTransceiverTransmitCbData *)user_data;
    EAS_ASSERT(data);
    if (data->cb) {
        data->cb(NOTIFY_SUCCESS, data->user_data);
    }
    transmit_cb_data_allocator_free(data);
}

static void transceiver_transmit(const uint8_t *const bytes, size_t num_bytes, TransceiverTransmitCompleteCb cb,
                                 void *user_data)
{
    NrfBleTransceiverTransmitCbData *transmit_cb_data = transmit_cb_data_allocator_alloc();
    EAS_ASSERT(transmit_cb_data);
    transmit_cb_data->cb = cb;
    transmit_cb_data->user_data = user_data;

    bool ret = eass_send(bytes, num_bytes, notify_complete_cb, (void *)transmit_cb_data);
    if (!ret) {
        EAS_LOG_INF("Failed to transmit");
    }
}

static void transceiver_set_receive_cb(TransceiverReceiveCb cb, void *user_data)
{
    receive_cb = cb;
    receive_cb_user_data = user_data;
}

static void transceiver_unset_receive_cb()
{
    receive_cb = NULL;
    receive_cb_user_data = NULL;
}

NrfBleTransceiverVirtualInterfaces virtual_transceiver_nrf_ble_initialize()
{
    EassCbs eass_cbs = {
        .received = eass_received_cb,
        .send_enabled = NULL,
    };
    eass_init(&eass_cbs);
    return (NrfBleTransceiverVirtualInterfaces){&transceiver};
}
