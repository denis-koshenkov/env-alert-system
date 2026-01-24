#include <string.h>
#include <stddef.h>

#include "nrfx_twim.h"

#include "interface.h"
#include "hw_platform_private.h"
#include "eas_assert.h"
#include "eas_timer.h"
#include "ops_queue.h"

/* From the implementation of BH1750 driver - a I2C write transaction never has length > 1 */
#define BH1750_DRIVER_I2C_WRITE_BUF_SIZE 1

void bh1750_driver_i2c_read(uint8_t *data, size_t length, uint8_t i2c_addr, void *user_data, BH1750_I2CCompleteCb cb,
                            void *cb_user_data)
{
    EAS_ASSERT(data);
    EAS_ASSERT(user_data);
    OpsQueue *i2c_queue_p = (OpsQueue *)user_data;

    I2cOperation i2c_op = {
        .complete_cb_data.cb_type = I2C_COMPLETE_CB_TYPE_BH1750,
        .complete_cb_data.cb.bh1750_cb = cb,
        .complete_cb_data.user_data = cb_user_data,
    };

    i2c_op.twim_xfer_desc.type = NRFX_TWIM_XFER_RX;
    i2c_op.twim_xfer_desc.address = i2c_addr;
    i2c_op.twim_xfer_desc.primary_length = length;
    i2c_op.twim_xfer_desc.secondary_length = 0;
    i2c_op.twim_xfer_desc.p_primary_buf = data;
    i2c_op.twim_xfer_desc.p_secondary_buf = NULL;

    ops_queue_add_op(*i2c_queue_p, &i2c_op);
}

void bh1750_driver_i2c_write(uint8_t *data, size_t length, uint8_t i2c_addr, void *user_data, BH1750_I2CCompleteCb cb,
                             void *cb_user_data)
{
    EAS_ASSERT(data);
    EAS_ASSERT(user_data);
    OpsQueue *i2c_queue_p = (OpsQueue *)user_data;

    I2cOperation i2c_op = {
        .complete_cb_data.cb_type = I2C_COMPLETE_CB_TYPE_BH1750,
        .complete_cb_data.cb.bh1750_cb = cb,
        .complete_cb_data.user_data = cb_user_data,
    };

    /* data pointer will point to invalid data once this function returns - BH1750 driver does not provide any
     * guarantees that it will keep pointing to valid memory. We make a copy of the data to be written, so that when the
     * i2c queue actually starts the operation, the data to be written points to a valid buffer. BH1750 driver will
     * never initiate another I2C write before the previous one is complete, so the data will not be overwritten by
     * another write. */
    static uint8_t write_buf[BH1750_DRIVER_I2C_WRITE_BUF_SIZE];
    EAS_ASSERT(length <= BH1750_DRIVER_I2C_WRITE_BUF_SIZE);
    memcpy(write_buf, data, length);

    i2c_op.twim_xfer_desc.type = NRFX_TWIM_XFER_TX;
    i2c_op.twim_xfer_desc.address = i2c_addr;
    i2c_op.twim_xfer_desc.primary_length = length;
    i2c_op.twim_xfer_desc.secondary_length = 0;
    i2c_op.twim_xfer_desc.p_primary_buf = write_buf;
    i2c_op.twim_xfer_desc.p_secondary_buf = NULL;

    ops_queue_add_op(*i2c_queue_p, &i2c_op);
}

void bh1750_driver_timer_start(uint32_t duration_ms, void *user_data, BH1750TimerExpiredCb cb, void *cb_user_data)
{
    BH1750TimerData *tmr_data = (BH1750TimerData *)user_data;
    EAS_ASSERT(tmr_data);

    tmr_data->cb = cb;
    tmr_data->user_data = cb_user_data;
    const EasTimer *const eas_timer_inst_p = tmr_data->eas_timer_inst_p;
    EAS_ASSERT(eas_timer_inst_p);

    eas_timer_set_period(*eas_timer_inst_p, duration_ms);
    eas_timer_start(*eas_timer_inst_p);
}
