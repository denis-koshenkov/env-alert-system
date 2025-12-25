#include <stddef.h>
#include <stdint.h>

#include <zephyr/drivers/i2c.h>
#include "nrfx_twim.h"

#include "hw_platform.h"
#include "led_nrf52840.h"
#include "eas_assert.h"
#include "virtual_sht31.h"
#include "virtual_bmp280.h"
#include "virtual_bh1750.h"
#include "eas_log.h"
#include "util.h"
#include "central_event_queue.h"
#include "eas_timer.h"

// TODO: remove, will come from its own repo
#include "sht31.h"

EAS_LOG_ENABLE_IN_FILE();

/* Convenience macros to pass to SHT31_I2CTransactionCompleteCb result parameter */
#define I2C_SUCCESS true
#define I2C_FAILURE false

/* Pin numbers are taken from nrf52840dk_nrf52840-pinctrl.dtsi */
#define I2C_SCL_PIN 27
#define I2C_SDA_PIN 26

#define SHT31_I2C_ADDR 0x44

#define TWIM_INST_IDX 0
static nrfx_twim_t twim_inst = NRFX_TWIM_INSTANCE(NRF_TWIM_INST_GET(TWIM_INST_IDX));
/* Initialized with some values so that these buffers are placed in the .data section - requirement from nrfx_twim_xfer
 * function */
// static uint8_t twim_tx_buffer[2] = {0x1, 0x0};
// static uint8_t twim_rx_buffer[6] = {0x1, 0x0, 0x1, 0x0, 0x1, 0x0};

static SHT31_I2CTransactionCompleteCb sht31_i2c_complete_cb = NULL;
static void *sht31_i2c_complete_cb_user_data = NULL;

/* SHT31 driver timer related */
static EasTimer sht31_driver_timer;
static SHT31TimerExpiredCb sht31_driver_timer_cb = NULL;
static void *sht31_driver_timer_cb_user_data = NULL;

static const TemperatureSensor *temperature_sensor = NULL;
static const HumiditySensor *humidity_sensor = NULL;
static const PressureSensor *pressure_sensor = NULL;
static const LightIntensitySensor *light_intensity_sensor = NULL;

static void submit_sht31_i2c_complete_cb_to_queue(bool result)
{
    /* If the driver did not specify a callback to be executed, we must not submit it to the central event queue. The
     * queue does not allow to submit NULL callbacks to be executed. */
    if (sht31_i2c_complete_cb) {
        central_event_queue_submit_void_cb_with_params_bool_user_data_event(sht31_i2c_complete_cb, result,
                                                                            sht31_i2c_complete_cb_user_data);
    }
}

/* Executes the callback that was set by the SHT31 driver. This function is a eas_timer expiry function, so it is
 * executed from the context of the central event queue. */
static void sht31_driver_timer_expired_cb(void *user_data)
{
    if (sht31_driver_timer_cb) {
        sht31_driver_timer_cb(sht31_driver_timer_cb_user_data);
    }
}

static void sht31_i2c_read(uint8_t *data, size_t length, uint8_t i2c_addr, SHT31_I2CTransactionCompleteCb cb,
                           void *user_data)
{
    EAS_ASSERT(data);

    sht31_i2c_complete_cb = cb;
    sht31_i2c_complete_cb_user_data = user_data;

    nrfx_twim_xfer_desc_t twim_xfer_desc_rx = NRFX_TWIM_XFER_DESC_RX(i2c_addr, data, length);
    int rc = nrfx_twim_xfer(&twim_inst, &twim_xfer_desc_rx, 0);
    EAS_ASSERT(rc == 0);
}

static void sht31_i2c_write(uint8_t *data, size_t length, uint8_t i2c_addr, SHT31_I2CTransactionCompleteCb cb,
                            void *user_data)
{
    EAS_ASSERT(data);

    sht31_i2c_complete_cb = cb;
    sht31_i2c_complete_cb_user_data = user_data;

    nrfx_twim_xfer_desc_t twim_xfer_desc_tx = NRFX_TWIM_XFER_DESC_TX(i2c_addr, data, length);
    int rc = nrfx_twim_xfer(&twim_inst, &twim_xfer_desc_tx, 0);
    EAS_ASSERT(rc == 0);
}

static void sht31_driver_timer_start(uint32_t duration_ms, SHT31TimerExpiredCb cb, void *user_data)
{
    sht31_driver_timer_cb = cb;
    sht31_driver_timer_cb_user_data = user_data;
    eas_timer_set_period(sht31_driver_timer, duration_ms);
    eas_timer_start(sht31_driver_timer);
}

static void twim_handler(nrfx_twim_event_t const *p_event, void *p_context)
{
    switch (p_event->type) {
    case NRFX_TWIM_EVT_DONE:
        EAS_LOG_INF("NRFX_TWIM_EVT_DONE");
        if (p_event->xfer_desc.type == NRFX_TWIM_XFER_RX) {
            EAS_LOG_HEXDUMP_INF(p_event->xfer_desc.p_primary_buf, p_event->xfer_desc.primary_length, "Received bytes");
        }
        submit_sht31_i2c_complete_cb_to_queue(I2C_SUCCESS);
        break;
    case NRFX_TWIM_EVT_ADDRESS_NACK:
        EAS_LOG_INF("NRFX_TWIM_EVT_ADDRESS_NACK");
        submit_sht31_i2c_complete_cb_to_queue(I2C_FAILURE);
        break;
    case NRFX_TWIM_EVT_DATA_NACK:
        EAS_LOG_INF("NRFX_TWIM_EVT_DATA_NACK");
        submit_sht31_i2c_complete_cb_to_queue(I2C_FAILURE);
        break;
    case NRFX_TWIM_EVT_OVERRUN:
        EAS_LOG_INF("NRFX_TWIM_EVT_OVERRUN");
        submit_sht31_i2c_complete_cb_to_queue(I2C_FAILURE);
        break;
    case NRFX_TWIM_EVT_BUS_ERROR:
        EAS_LOG_INF("NRFX_TWIM_EVT_BUS_ERROR");
        submit_sht31_i2c_complete_cb_to_queue(I2C_FAILURE);
        break;
    default:
        /* Invalid TWIM event */
        EAS_ASSERT(0);
        break;
    }
}

static void init_nrfx_twim()
{
    IRQ_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_TWIM_INST_GET(TWIM_INST_IDX)), IRQ_PRIO_LOWEST, nrfx_twim_irq_handler,
                &twim_inst, 0);

    int rc = 0;

    nrfx_twim_config_t twim_config = {
        .scl_pin = I2C_SCL_PIN,
        .sda_pin = I2C_SDA_PIN,
        /* TWIM does not support 1000K */
        .frequency = NRF_TWIM_FREQ_400K,
        /* Taken from NRFX_TWIM_DEFAULT_CONFIG() */
        .interrupt_priority = NRFX_TWIM_DEFAULT_CONFIG_IRQ_PRIORITY,
        /* Taken from NRFX_TWIM_DEFAULT_CONFIG() */
        .hold_bus_uninit = false,
        /* Since we are not using i2c in Zephyr, gpios will be initialized by nrfx_twim driver */
        .skip_gpio_cfg = false,
        .skip_psel_cfg = false,
    };
    nrfx_twim_init(&twim_inst, &twim_config, twim_handler, NULL);
    EAS_ASSERT(rc == 0);
    EAS_LOG_INF("Initialized nrfx twim");

    nrfx_twim_enable(&twim_inst);
    EAS_LOG_INF("Enabled nrfx twim");

    // /* High repeateability measurement command with clock stretching disabled */
    // twim_tx_buffer[0] = 0x24;
    // twim_tx_buffer[1] = 0x0;
    // nrfx_twim_xfer_desc_t twim_xfer_desc_tx = NRFX_TWIM_XFER_DESC_TX(SHT31_I2C_ADDR, twim_tx_buffer, 2);
    // rc = nrfx_twim_xfer(&twim_inst, &twim_xfer_desc_tx, 0);
    // EAS_ASSERT(rc == 0);

    // /* Max measurement time is 15.5 ms according to datashseet, wait for 20 ms to allow for margin */
    // k_sleep(K_MSEC(20));

    // /* Read measurement results */
    // nrfx_twim_xfer_desc_t twim_xfer_desc_rx = NRFX_TWIM_XFER_DESC_RX(SHT31_I2C_ADDR, twim_rx_buffer, 6);
    // rc = nrfx_twim_xfer(&twim_inst, &twim_xfer_desc_rx, 0);
    // EAS_ASSERT(rc == 0);
}

void hw_platform_init()
{
    init_nrfx_twim();
    sht31_driver_timer = eas_timer_create(0, EAS_TIMER_ONE_SHOT, sht31_driver_timer_expired_cb, NULL);
    SHT31InitConfig sht31_cfg = {
        .i2c_read = sht31_i2c_read,
        .i2c_write = sht31_i2c_write,
        .start_timer = sht31_driver_timer_start,
        .i2c_addr = SHT31_I2C_ADDR,
    };
    sht31_init(&sht31_cfg);

    SHT31VirtualInterfaces sht31_interfaces = virtual_sht31_initialize();
    temperature_sensor = sht31_interfaces.temperature_sensor;
    humidity_sensor = sht31_interfaces.humidity_sensor;

    BMP280VirtualInterfaces bmp280_interfaces = virtual_bmp280_initialize();
    pressure_sensor = bmp280_interfaces.pressure_sensor;

    BH1750VirtualInterfaces bh1750_interfaces = virtual_bh1750_initialize();
    light_intensity_sensor = bh1750_interfaces.light_intensity_sensor;
}

const Led *const hw_platform_get_led()
{
    return led_nrf52840_get();
}

const TemperatureSensor *const hw_platform_get_temperature_sensor()
{
    EAS_ASSERT(temperature_sensor);
    return temperature_sensor;
}

const PressureSensor *const hw_platform_get_pressure_sensor()
{
    EAS_ASSERT(pressure_sensor);
    return pressure_sensor;
}

const HumiditySensor *const hw_platform_get_humidity_sensor()
{
    EAS_ASSERT(humidity_sensor);
    return humidity_sensor;
}

const LightIntensitySensor *const hw_platform_get_light_intensity_sensor()
{
    EAS_ASSERT(light_intensity_sensor);
    return light_intensity_sensor;
}
