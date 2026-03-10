#ifndef ENV_ALERT_SYSTEM_SRC_HAL_IMPL_NRF52840DK_DRIVERS_GLUE_BMP280_INTERFACE_H
#define ENV_ALERT_SYSTEM_SRC_HAL_IMPL_NRF52840DK_DRIVERS_GLUE_BMP280_INTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "bmp280.h"

/**
 * @brief Read regs function provided to BMP280 driver.
 *
 * @param[in] start_addr Register address to read the first value from.
 * @param[in] num_regs Number of registers to read.
 * @param[out] data Must be a buffer of @p num_regs bytes (each register value is 1 byte). Data that was read is written
 * to this parameter.
 * @param[in] user_data When this function is called, this parameter is equal to the read_regs_user_data passed to
 * bmp280_create function in the init cfg.
 * @param[in] cb Callback that must be executed once the read is complete. Must be invoked from the same context as
 * other BMP280 driver functions.
 * @param[in] cb_user_data User data to pass to @p cb.
 */
void bmp280_driver_read_regs(uint8_t start_addr, size_t num_regs, uint8_t *data, void *user_data,
                             BMP280_IOCompleteCb cb, void *cb_user_data);

/**
 * @brief Write reg function provided to BMP280 driver.
 *
 * @param[in] addr Address of register to write.
 * @param[in] reg_val Value to write to the register.
 * @param[in] user_data When this function is called, this parameter is equal to the write_reg_user_data passed to
 * bmp280_create function in the init cfg.
 * @param[in] cb Callback that must be executed once the write is complete. Must be invoked from the same context as
 * other BMP280 driver functions.
 * @param[in] cb_user_data User data to pass to @p cb.
 */
void bmp280_driver_write_reg(uint8_t addr, uint8_t reg_val, void *user_data, BMP280_IOCompleteCb cb,
                             void *cb_user_data);

/**
 * @brief Start timer function provided to BMP280 driver. Invokes @p cb after at least @p duration_ms ms pass.
 *
 * @param[in] duration_ms Timer period. @p cb must be invoked after at least this number of ms pass.
 * @param[in] user_data User data. This parameter is equal to the start_timer_user_data passed to bmp280_create
 * function in the init cfg.
 * @param[in] cb Callback to execute when the timer expires. Must be invoked from the same context as other BMP280
 * driver functions.
 * @param[in] cb_user_data User data to pass to @p cb.
 */
void bmp280_driver_start_timer(uint32_t duration_ms, void *user_data, BMP280TimerExpiredCb cb, void *cb_user_data);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_HAL_IMPL_NRF52840DK_DRIVERS_GLUE_BMP280_INTERFACE_H */
