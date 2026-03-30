#ifndef ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_NRF_BLE_TRANSMIT_CB_DATA_ALLOCATOR_H
#define ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_NRF_BLE_TRANSMIT_CB_DATA_ALLOCATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "transmit_cb_data_def.h"

/**
 * @brief Block memory allocator for nrf BLE transceiver transmit data.
 *
 * The transmit data includes the callback to execute once transmission is complete, and the user data to pass to the
 * callback.
 *
 * The implementation of this interface must be able to simulatensously allocate
 * CONFIG_TRANSMIT_CB_DATA_ALLOCATOR_NUM_BLOCKS memory blocks of size sizeof(NrfBleTransceiverTransmitCbData) bytes.
 */

/**
 * @brief Allocate memory for one transmit cb data.
 *
 * @return NrfBleTransceiverTransmitCbData* If successful, points to allocated memory for transmit cb data. If failed
 * due to being out of memory, returns NULL.
 */
NrfBleTransceiverTransmitCbData *transmit_cb_data_allocator_alloc();

/**
 * @brief Free a previously allocated transmit cb data.
 *
 * @param transmit_cb_data Transmit cb data buffer previously returned by @ref transmit_cb_data_allocator_alloc.
 */
void transmit_cb_data_allocator_free(NrfBleTransceiverTransmitCbData *transmit_cb_data);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_NRF_BLE_TRANSMIT_CB_DATA_ALLOCATOR_H */
