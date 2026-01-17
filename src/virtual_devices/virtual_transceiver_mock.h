#ifndef ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_TRANSCEIVER_MOCK_H
#define ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_TRANSCEIVER_MOCK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "transceiver.h"

/**
 * @brief Returns the transceiver interface with function pointers initialized to functions that implement the
 * transceiver interface using mock CppUMock functions.
 *
 * @return const Transceiver* const Mock transceiver interface.
 */
const Transceiver *const virtual_transceiver_mock_get();

/**
 * @brief Resets transmit complete cbs array index to 0.
 *
 * After a call to this function, the next call to @ref transceiver_transmit will write the
 * TransceiverTransmitCompleteCb and its user data to the array of transmit complete cbs at index 0.
 */
void virtual_transceiver_mock_reset_cbs_index();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_TRANSCEIVER_MOCK_H */
