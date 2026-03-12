#ifndef ENV_ALERT_SYSTEM_SRC_HAL_IMPL_NRF52840DK_BLE_EASS_H
#define ENV_ALERT_SYSTEM_SRC_HAL_IMPL_NRF52840DK_BLE_EASS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

// #include <zephyr/types.h>
// #include <zephyr/bluetooth/conn.h>

/** @brief EASS Service UUID. */
#define BT_UUID_EASS_VAL BT_UUID_128_ENCODE(0x7CEC6BFA, 0x6B62, 0x4371, 0x99EB, 0xA335315223C5)

/** @brief TX Characteristic UUID. */
#define BT_UUID_EASS_TX_VAL BT_UUID_128_ENCODE(0x7CEC6BFB, 0x6B62, 0x4371, 0x99EB, 0xA335315223C5)

/** @brief RX Characteristic UUID. */
#define BT_UUID_EASS_RX_VAL BT_UUID_128_ENCODE(0x7CEC6BFC, 0x6B62, 0x4371, 0x99EB, 0xA335315223C5)

#define BT_UUID_EASS BT_UUID_DECLARE_128(BT_UUID_EASS_VAL)
#define BT_UUID_EASS_TX BT_UUID_DECLARE_128(BT_UUID_EASS_TX_VAL)
#define BT_UUID_EASS_RX BT_UUID_DECLARE_128(BT_UUID_EASS_RX_VAL)

/** @brief Pointers to the callback functions for service events. */
typedef struct {
    /** @brief Data received callback.
     *
     * @param[in] data Received data.
     * @param[in] len Length of received data.
     */
    void (*received)(const uint8_t *const data, uint16_t len);

    /** @brief Send state callback.
     *
     * Indicate the CCCD descriptor status of the EASS TX characteristic.
     *
     * @param[in] enabled True if notifications are enabled, false otherwise.
     */
    void (*send_enabled)(bool enabled);
} EassCbs;

/** @brief Initialize the EASS service.
 *
 * @param[in] cbs Struct with function pointers to callbacks for service events. If no callbacks are needed, this
 * parameter can be NULL.
 */
void eass_init(const EassCbs *const cbs);

/** @brief Send data.
 *
 * @param[in] data Pointer to a data buffer.
 * @param[in] len  Length of the data in the buffer.
 *
 * @retval true If the data is sent successfully.
 * @retval false If failed to send.
 */
bool eass_send(const uint8_t *data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_HAL_IMPL_NRF52840DK_BLE_EASS_H */
