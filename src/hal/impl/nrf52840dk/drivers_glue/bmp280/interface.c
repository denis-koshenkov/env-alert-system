#include <zephyr/drivers/spi.h>

#include "interface.h"
#include "hw_platform_private.h"
#include "eas_assert.h"
#include "eas_timer.h"

#define SPI_RW_BIT_MASK ((uint8_t)(((uint8_t)1U) << 7))

void bmp280_driver_read_regs(uint8_t start_addr, size_t num_regs, uint8_t *data, void *user_data,
                             BMP280_IOCompleteCb cb, void *cb_user_data)
{
    EAS_ASSERT(user_data);
    SpiOpData *spi_op_data_p = (SpiOpData *)user_data;
    const struct spi_dt_spec *spi_spec = spi_op_data_p->spi_spec;

    spi_op_data_p->cb = cb;
    spi_op_data_p->user_data = cb_user_data;

    /* Give address of a static variable to async spi_transceive_cb instead of local stack variable */
    static uint8_t start_addr_copy;
    start_addr_copy = start_addr;

    /* For SPI read, the MSB of register address must always be 1 to indicate a read operation */
    start_addr_copy |= SPI_RW_BIT_MASK;

    static struct spi_buf tx_spi_buf;
    static const struct spi_buf_set tx_buf_set = {
        .buffers = &tx_spi_buf,
        .count = 1,
    };
    /* Transmit one byte - starting register address */
    tx_spi_buf.buf = &start_addr_copy;
    tx_spi_buf.len = 1;

    static struct spi_buf rx_spi_bufs[2] = {
        /* First buffer always indicates a RX NOP of 1 byte, because the first byte of the transaction is a TX byte -
           register address. */
        {.buf = NULL, .len = 1},
        {.buf = NULL, .len = 0},
    };
    static const struct spi_buf_set rx_buf_set = {
        .buffers = rx_spi_bufs,
        .count = 2,
    };
    rx_spi_bufs[1].buf = data;
    rx_spi_bufs[1].len = num_regs;

    int rc = spi_transceive_cb(spi_spec->bus, &spi_spec->config, &tx_buf_set, &rx_buf_set, spi_op_data_p->zephyr_spi_cb,
                               NULL);
    EAS_ASSERT(rc == 0);
}

void bmp280_driver_write_reg(uint8_t addr, uint8_t reg_val, void *user_data, BMP280_IOCompleteCb cb, void *cb_user_data)
{
    EAS_ASSERT(user_data);
    SpiOpData *spi_op_data_p = (SpiOpData *)user_data;
    const struct spi_dt_spec *spi_spec = spi_op_data_p->spi_spec;

    spi_op_data_p->cb = cb;
    spi_op_data_p->user_data = cb_user_data;

    /* Give addresses of a static variables to async spi_transceive_cb instead of local stack variables */
    static uint8_t addr_copy;
    static uint8_t reg_val_copy;
    addr_copy = addr;
    reg_val_copy = reg_val;

    /* For SPI write, the MSB of register address must always be 0 to indicate a write operation */
    addr_copy &= ~SPI_RW_BIT_MASK;

    static struct spi_buf spi_bufs[2];
    static const struct spi_buf_set tx_buf_set = {
        .buffers = spi_bufs,
        .count = 2,
    };
    /* Register address buf */
    spi_bufs[0].buf = &addr_copy;
    spi_bufs[0].len = 1;
    /* Register value buf */
    spi_bufs[1].buf = &reg_val_copy;
    spi_bufs[1].len = 1;

    int rc = spi_transceive_cb(spi_spec->bus, &spi_spec->config, &tx_buf_set, NULL, spi_op_data_p->zephyr_spi_cb, NULL);
    EAS_ASSERT(rc == 0);
}

void bmp280_driver_start_timer(uint32_t duration_ms, void *user_data, BMP280TimerExpiredCb cb, void *cb_user_data)
{
    BMP280TimerData *tmr_data = (BMP280TimerData *)user_data;
    EAS_ASSERT(tmr_data);

    tmr_data->cb = cb;
    tmr_data->user_data = cb_user_data;
    const EasTimer *const eas_timer_inst_p = tmr_data->eas_timer_inst_p;
    EAS_ASSERT(eas_timer_inst_p);

    eas_timer_set_period(*eas_timer_inst_p, duration_ms);
    eas_timer_start(*eas_timer_inst_p);
}
