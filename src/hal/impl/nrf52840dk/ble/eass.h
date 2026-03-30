#ifndef ENV_ALERT_SYSTEM_SRC_HAL_IMPL_NRF52840DK_BLE_EASS_H
#define ENV_ALERT_SYSTEM_SRC_HAL_IMPL_NRF52840DK_BLE_EASS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

/** @brief EASS Service UUID. */
#define BT_UUID_EASS_VAL BT_UUID_128_ENCODE(0x7CEC6BFA, 0x6B62, 0x4371, 0x99EB, 0xA335315223C5)

/** @brief TX Characteristic UUID. */
#define BT_UUID_EASS_TX_VAL BT_UUID_128_ENCODE(0x7CEC6BFB, 0x6B62, 0x4371, 0x99EB, 0xA335315223C5)

/** @brief RX Characteristic UUID. */
#define BT_UUID_EASS_RX_VAL BT_UUID_128_ENCODE(0x7CEC6BFC, 0x6B62, 0x4371, 0x99EB, 0xA335315223C5)

#define BT_UUID_EASS BT_UUID_DECLARE_128(BT_UUID_EASS_VAL)
#define BT_UUID_EASS_TX BT_UUID_DECLARE_128(BT_UUID_EASS_TX_VAL)
#define BT_UUID_EASS_RX BT_UUID_DECLARE_128(BT_UUID_EASS_RX_VAL)

/** Pointers to the callback functions for service events. */
typedef struct {
    /** @brief Callback to execute when data is received.
     *
     * @param[in] data Received data.
     * @param[in] len Length of received data.
     */
    void (*received)(const uint8_t *data, uint16_t len);

    /** @brief Callback to execute when sending is enabled/disabled.
     *
     * Callback to execute when someone enabled/disables notifications of the EASS TX characteristic. It is only
     * possible to send data if notifications are enabled.
     *
     * @param[in] enabled True if notifications are enabled, false otherwise.
     */
    void (*send_enabled)(bool enabled);
} EassCbs;

/** @brief Initialize the EASS service.
 *
 * @param[in] cbs Callbacks to execute on various events. If no callbacks are needed, this parameter can be NULL. This
 * can point to a stack variable that goes out of scope after this function returns - this function makes a copy of the
 * callbacks.
 */
void eass_init(const EassCbs *const cbs);

/** @brief Send data.
 *
 * Use the send_enabled callback to see if sending data is allowed. If sending data is not allowed at the moment, this
 * function will always return false and not send any data.
 *
 * @note It is allowed for @p data to point to stack memory. This function will make a copy of the data to be sent, so
 * it is ok if @p data goes out of scope after this function returns.
 *
 * @param[in] data Pointer to a data buffer.
 * @param[in] len  Length of the data in the buffer.
 *
 * @retval true If the data is sent successfully.
 * @retval false If failed to send.
 */
bool eass_send(const uint8_t *data, uint16_t len, bt_gatt_complete_func_t cb, void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_HAL_IMPL_NRF52840DK_BLE_EASS_H */
