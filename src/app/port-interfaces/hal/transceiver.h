#ifndef ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HAL_TRANSCEIVER_H
#define ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HAL_TRANSCEIVER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Interface to send and receive bytes over a connectivity medium.
 *
 * Abstracts away the actual connectivity medium used.
 */

/**
 * @brief Callback to execute when a transmission operation is complete.
 *
 * @param result True if transmission was successful, false if transmission failed.
 * @param user_data User data.
 */
typedef void (*TransceiverTransmitCompleteCb)(bool result, void *user_data);

/**
 * @brief Callback to execute when bytes are received over the connectivity medium.
 *
 * @param bytes An array of received bytes.
 * @param num_bytes Number of bytes in the @p bytes array.
 * @param user_data User data.
 */
typedef void (*TransceiverReceiveCb)(uint8_t *bytes, size_t num_bytes, void *user_data);

/**
 * @brief Transmit bytes.
 *
 * @param bytes An array of bytes to transmit. The first @p num_bytes at this memory address will be transmitted.
 * @param num_bytes Number of bytes in the @p bytes array.
 * @param cb Callback to execute when transmission is complete - either successfully or unsuccessfully.
 * @param user_data User data to pass to @p cb when it is executed.
 */
void transceiver_transmit(const uint8_t *const bytes, size_t num_bytes, TransceiverTransmitCompleteCb cb,
                          void *user_data);

/**
 * @brief Set callback to execute when bytes are received over the connectivity medium.
 *
 * @param cb Callback to execute.
 * @param user_data User data to pass to @p cb when it is executed.
 */
void transceiver_set_receive_cb(TransceiverReceiveCb cb, void *user_data);

/**
 * @brief Unset callback to execute when bytes are received over the connectivity medium.
 *
 * After a call to this function, the implementation should do nothing when it receives bytes over the connectivity
 * medium.
 */
void transceiver_unset_receive_cb();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HAL_TRANSCEIVER_H */
