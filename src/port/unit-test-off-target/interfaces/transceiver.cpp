#include "CppUTestExt/MockSupport.h"
#include "hal/transceiver.h"

void transceiver_transmit(const uint8_t *const bytes, size_t num_bytes, TransceiverTransmitCompleteCb cb,
                          void *user_data)
{
    /* Give cb and user_data to the test by populating the provided pointers, so that the test can simulate calling the
     * TransmitCompleteCb */
    TransceiverTransmitCompleteCb *transmit_complete_cb_pointer =
        (TransceiverTransmitCompleteCb *)mock().getData("transmitCompleteCb").getPointerValue();
    void **transmit_complete_cb_user_data_pointer =
        (void **)mock().getData("transmitCompleteCbUserData").getPointerValue();
    *transmit_complete_cb_pointer = cb;
    *transmit_complete_cb_user_data_pointer = user_data;

    mock()
        .actualCall("transceiver_transmit")
        .withMemoryBufferParameter("bytes", bytes, num_bytes)
        .withParameter("num_bytes", num_bytes)
        .withParameter("cb", cb)
        .withParameter("user_data", user_data);
}

void transceiver_set_receive_cb(TransceiverReceiveCb cb, void *user_data)
{
    mock().actualCall("transceiver_set_receive_cb").withParameter("cb", cb).withParameter("user_data", user_data);
}

void transceiver_unset_receive_cb()
{
    mock().actualCall("transceiver_unset_receive_cb");
}
