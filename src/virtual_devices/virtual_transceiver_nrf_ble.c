#include "virtual_transceiver_nrf_ble.h"

static void transceiver_transmit(const uint8_t *const bytes, size_t num_bytes, TransceiverTransmitCompleteCb cb,
                                 void *user_data);
static void transceiver_set_receive_cb(TransceiverReceiveCb cb, void *user_data);
static void transceiver_unset_receive_cb();

static Transceiver transceiver = {
    .transmit = transceiver_transmit,
    .set_receive_cb = transceiver_set_receive_cb,
    .unset_receive_cb = transceiver_unset_receive_cb,
};

static void transceiver_transmit(const uint8_t *const bytes, size_t num_bytes, TransceiverTransmitCompleteCb cb,
                                 void *user_data)
{
}

static void transceiver_set_receive_cb(TransceiverReceiveCb cb, void *user_data)
{
}

static void transceiver_unset_receive_cb()
{
}

const Transceiver *const virtual_transceiver_nrf_ble_get()
{
    return &transceiver;
}
