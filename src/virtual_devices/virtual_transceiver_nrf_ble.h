#ifndef ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_TRANSCEIVER_NRF_BLE_H
#define ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_TRANSCEIVER_NRF_BLE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "transceiver.h"

/**
 * @brief Returns the transceiver interface with function pointers initialized to functions that implement the
 * transceiver interface using nRF BLE.
 *
 * @return const Transceiver* const nRF BLE transceiver interface.
 */
const Transceiver *const virtual_transceiver_nrf_ble_get();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_TRANSCEIVER_NRF_BLE_H */
