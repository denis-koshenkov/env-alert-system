#ifndef ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_NRF_BLE_TRANSMIT_CB_DATA_DEF_H
#define ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_NRF_BLE_TRANSMIT_CB_DATA_DEF_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "transceiver.h"

typedef struct {
    /** Callback to execute once transmission is complete. */
    TransceiverTransmitCompleteCb cb;
    /** User data to pass to cb as parameter. */
    void *user_data;
} NrfBleTransceiverTransmitCbData;

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_NRF_BLE_TRANSMIT_CB_DATA_DEF_H */
