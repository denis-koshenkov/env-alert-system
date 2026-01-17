#ifndef ENV_ALERT_SYSTEM_SRC_HAL_IMPL_UNIT_TEST_OFF_TARGET_MOCK_TRANSCEIVER_H
#define ENV_ALERT_SYSTEM_SRC_HAL_IMPL_UNIT_TEST_OFF_TARGET_MOCK_TRANSCEIVER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "hal/transceiver.h"

/**
 * @brief Defines an extension to the transceiver interface for the unit test port.
 *
 * This extension interface defines a function to reset transceiver mock object, so that it starts populating the
 * transmit complete cbs array at index 0. Since this function should not be a part of the port interface defined by the
 * app, it is defined here in this extension header. Unit tests should include this header instead of the
 * transceiver.h one.
 */

/**
 * @brief Resets transmit complete cbs array index to 0.
 *
 * After a call to this function, the next call to @ref transceiver_transmit will write the
 * TransceiverTransmitCompleteCb and its user data to the array of transmit complete cbs at index 0.
 */
void mock_transceiver_reset_cbs_index();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_HAL_IMPL_UNIT_TEST_OFF_TARGET_MOCK_TRANSCEIVER_H */
