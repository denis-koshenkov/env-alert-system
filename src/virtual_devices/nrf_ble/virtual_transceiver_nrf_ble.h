#ifndef ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_NRF_BLE_VIRTUAL_TRANSCEIVER_NRF_BLE_H
#define ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_NRF_BLE_VIRTUAL_TRANSCEIVER_NRF_BLE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "transceiver.h"

/** All virtual devices that are based on nrf BLE. */
typedef struct NrfBleTransceiverVirtualInterfaces {
    const Transceiver *const transceiver;
} NrfBleTransceiverVirtualInterfaces;

/**
 * @brief Initialize virtual devices for the nrf BLE transceiver implementation.
 *
 * @pre BLE is initialized. EASS service is NOT initialized. EASS service is initialized by this function.
 *
 * @return NrfBleTransceiverVirtualInterfaces Initialized virtual device interfaces that contain valid, ready-to-use
 * function pointers.
 */
NrfBleTransceiverVirtualInterfaces virtual_transceiver_nrf_ble_initialize();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_NRF_BLE_VIRTUAL_TRANSCEIVER_NRF_BLE_H */
