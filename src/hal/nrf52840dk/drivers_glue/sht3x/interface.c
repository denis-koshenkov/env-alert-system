#include "nrfx_twim.h"

#include "interface.h"
#include "hw_platform_private.h"
#include "eas_assert.h"
#include "eas_timer.h"

void sht3x_driver_i2c_read(uint8_t *data, size_t length, uint8_t i2c_addr, void *user_data,
                           SHT3X_I2CTransactionCompleteCb cb, void *cb_user_data)
{
    EAS_ASSERT(data);
    EAS_ASSERT(user_data);

    SHT3XDriverI2cData *i2c_data = (SHT3XDriverI2cData *)user_data;
    i2c_data->cb = cb;
    i2c_data->user_data = cb_user_data;
    nrfx_twim_t *p_twim_inst = i2c_data->p_twim_inst;

    nrfx_twim_xfer_desc_t twim_xfer_desc_rx = NRFX_TWIM_XFER_DESC_RX(i2c_addr, data, length);
    int rc = nrfx_twim_xfer(p_twim_inst, &twim_xfer_desc_rx, 0);
    EAS_ASSERT(rc == 0);
}

void sht3x_driver_i2c_write(uint8_t *data, size_t length, uint8_t i2c_addr, void *user_data,
                            SHT3X_I2CTransactionCompleteCb cb, void *cb_user_data)
{
    EAS_ASSERT(data);
    EAS_ASSERT(user_data);

    SHT3XDriverI2cData *i2c_data = (SHT3XDriverI2cData *)user_data;
    i2c_data->cb = cb;
    i2c_data->user_data = cb_user_data;
    nrfx_twim_t *p_twim_inst = i2c_data->p_twim_inst;

    nrfx_twim_xfer_desc_t twim_xfer_desc_tx = NRFX_TWIM_XFER_DESC_TX(i2c_addr, data, length);
    int rc = nrfx_twim_xfer(p_twim_inst, &twim_xfer_desc_tx, 0);
    EAS_ASSERT(rc == 0);
}

void sht3x_driver_timer_start(uint32_t duration_ms, void *user_data, SHT3XTimerExpiredCb cb, void *cb_user_data)
{
    SHT3XTimerData *tmr_data = (SHT3XTimerData *)user_data;

    tmr_data->cb = cb;
    tmr_data->user_data = cb_user_data;
    const EasTimer *const p_eas_timer_inst = tmr_data->p_eas_timer_inst;

    eas_timer_set_period(*p_eas_timer_inst, duration_ms);
    eas_timer_start(*p_eas_timer_inst);
}
