#ifndef ENV_ALERT_SYSTEM_SRC_HAL_NRF52840DK_DRIVERS_GLUE_SHT3X_INTERFACE_H
#define ENV_ALERT_SYSTEM_SRC_HAL_NRF52840DK_DRIVERS_GLUE_SHT3X_INTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sht3x.h"

/**
 * @brief I2C read function provided to SHT3X driver.
 *
 * @param[out] data Must be a buffer of @p length bytes. Data that was read is written to this parameter.
 * @param[in] length Number of bytes to read.
 * @param[in] i2c_addr I2C device address.
 * @param[in] user_data User data. This parameter is equal to the i2c_read_user_data passed to sht3x_create function in
 * the init cfg.
 * @param[in] cb Callback that must be executed once the I2C read is complete. Must be invoked from the same context
 * as other SHT3X functions.
 * @param[in] cb_user_data User data to pass to @p cb.
 */
void sht3x_driver_i2c_read(uint8_t *data, size_t length, uint8_t i2c_addr, void *user_data,
                           SHT3X_I2CTransactionCompleteCb cb, void *cb_user_data);

/**
 * @brief I2C write function provided to SHT3X driver.
 *
 * @param[in] data Data to write. Must be a buffer of @p length bytes.
 * @param[in] length Number of bytes to write.
 * @param[in] i2c_addr I2C device address.
 * @param[in] user_data User data. This parameter is equal to the i2c_write_user_data passed to sht3x_create function in
 * the init cfg.
 * @param[in] cb Callback that must be executed once the I2C write is complete. Must be invoked from the same
 * context as other SHT3X functions.
 * @param[in] cb_user_data User data to pass to @p cb.
 */
void sht3x_driver_i2c_write(uint8_t *data, size_t length, uint8_t i2c_addr, void *user_data,
                            SHT3X_I2CTransactionCompleteCb cb, void *cb_user_data);

/**
 * @brief Timer start function provided to SHT3X driver. Invokes @p cb after at least @p duration_ms ms pass.
 *
 * @param[in] duration_ms Timer period. @p cb must be invoked after at least this number of ms pass.
 * @param[in] user_data User data. This parameter is equal to the timer_start_user_data passed to sht3x_create function
 * in the init cfg.
 * @param[in] cb Callback to execute when the timer expires. Must be invoked from the same context as other SHT3X
 * functions.
 * @param[in] cb_user_data User data to pass to @p cb.
 */
void sht3x_driver_timer_start(uint32_t duration_ms, void *user_data, SHT3XTimerExpiredCb cb, void *cb_user_data);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_HAL_NRF52840DK_DRIVERS_GLUE_SHT3X_INTERFACE_H */
