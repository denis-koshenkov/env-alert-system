#ifndef ENV_ALERT_SYSTEM_SRC_HAL_DEF_TRANSCEIVER_H
#define ENV_ALERT_SYSTEM_SRC_HAL_DEF_TRANSCEIVER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Callback to execute when a transmission operation is complete.
 *
 * @param[in] result True if transmission was successful, false if transmission failed.
 * @param[in] user_data User data.
 */
typedef void (*TransceiverTransmitCompleteCb)(bool result, void *user_data);

/**
 * @brief Callback to execute when bytes are received over the connectivity medium.
 *
 * @param[in] bytes An array of received bytes.
 * @param[in] num_bytes Number of bytes in the @p bytes array.
 * @param[in] user_data User data.
 */
typedef void (*TransceiverReceiveCb)(const uint8_t *bytes, size_t num_bytes, void *user_data);

/**
 * @brief Interface to send and receive bytes over a connectivity medium.
 *
 * Abstracts away the actual connectivity medium used.
 */
typedef struct {
    /**
     * @brief Transmit bytes.
     *
     * The implementation must be non-blocking. This function must initiate the transmission but not wait until it is
     * complete. When the transmission is complete, the callback set via "set_transmit_complete_cb" must be executed.
     *
     * @param[in] bytes An array of bytes to transmit. The first @p num_bytes at this memory address will be
     * transmitted.
     * @param[in] num_bytes Number of bytes in the @p bytes array.
     * @param[in] cb Callback to execute when transmission is complete - either successfully or unsuccessfully.
     * @param[in] user_data User data to pass to @p cb when it is executed.
     */
    void (*transmit)(const uint8_t *const bytes, size_t num_bytes, TransceiverTransmitCompleteCb cb, void *user_data);

    /**
     * @brief Set callback to execute when bytes are received over the connectivity medium.
     *
     * @param[in] cb Callback to execute.
     * @param[in] user_data User data to pass to @p cb when it is executed.
     */
    void (*set_receive_cb)(TransceiverReceiveCb cb, void *user_data);

    /**
     * @brief Unset callback to execute when bytes are received over the connectivity medium.
     *
     * After a call to this function, the implementation should do nothing when it receives bytes over the connectivity
     * medium.
     */
    void (*unset_receive_cb)();
} Transceiver;

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_HAL_DEF_TRANSCEIVER_H */
