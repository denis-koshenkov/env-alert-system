#ifndef ENV_ALERT_SYSTEM_SRC_PORT_NRF52840DK_INTERFACES_HAL_DRIVERS_GLUE_SHT3X_INTERFACE_H
#define ENV_ALERT_SYSTEM_SRC_PORT_NRF52840DK_INTERFACES_HAL_DRIVERS_GLUE_SHT3X_INTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sht3x.h"

void sht3x_driver_i2c_read(uint8_t *data, size_t length, uint8_t i2c_addr, void *user_data,
                           SHT3X_I2CTransactionCompleteCb cb, void *cb_user_data);

void sht3x_driver_i2c_write(uint8_t *data, size_t length, uint8_t i2c_addr, void *user_data,
                            SHT3X_I2CTransactionCompleteCb cb, void *cb_user_data);

void sht3x_driver_timer_start(uint32_t duration_ms, void *user_data, SHT3XTimerExpiredCb cb, void *cb_user_data);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_NRF52840DK_INTERFACES_HAL_DRIVERS_GLUE_SHT3X_INTERFACE_H */
