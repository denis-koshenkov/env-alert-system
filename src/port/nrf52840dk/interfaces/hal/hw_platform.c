#include <stddef.h>
#include <stdint.h>

#include "nrfx_twim.h"

#include "hw_platform.h"
#include "hw_platform_private.h"
#include "led_nrf52840.h"
#include "eas_assert.h"
#include "virtual_sht31.h"
#include "virtual_bmp280.h"
#include "virtual_bh1750.h"
#include "central_event_queue.h"
#include "eas_timer.h"
#include "sht3x.h"
#include "sht3x/get_instance_memory.h"
#include "sht3x/interface.h"

/* Pin numbers are taken from nrf52840dk_nrf52840-pinctrl.dtsi */
#define I2C_SCL_PIN 27
#define I2C_SDA_PIN 26
/* TWIM does not support 1000K */
#define I2C_TWIM_FREQ NRF_TWIM_FREQ_400K

#define SHT31_I2C_ADDR 0x44

/** @brief I2C result codes that describe the outcome of an I2C transaction. */
typedef enum {
    /** @brief I2C transaction successful. */
    I2C_RESULT_CODE_OK,
    /** @brief Generic I2C error. */
    I2C_RESULT_CODE_ERR,
    /** @brief NACK after sending the address byte. */
    I2C_RESULT_CODE_ADDR_NACK,
} I2cResultCode;

SHT3X sht3x_inst;

/* SHT31 driver I2C related */
#define TWIM_INST_IDX 0
static nrfx_twim_t twim_inst = NRFX_TWIM_INSTANCE(NRF_TWIM_INST_GET(TWIM_INST_IDX));
static SHT3XDriverI2cData sht3x_driver_i2c_data = {
    .cb = NULL,
    .user_data = NULL,
    .p_twim_inst = &twim_inst,
};

/* SHT31 driver timer related */
static EasTimer sht31_driver_timer;
static SHT3XTimerData sht3x_timer_data = {
    .cb = NULL,
    .user_data = NULL,
    .p_eas_timer_inst = &sht31_driver_timer,
};

static const TemperatureSensor *temperature_sensor = NULL;
static const HumiditySensor *humidity_sensor = NULL;
static const PressureSensor *pressure_sensor = NULL;
static const LightIntensitySensor *light_intensity_sensor = NULL;

/* Executes the callback that was set by SHT3X driver. This function is a eas_timer expiry function, so it is
 * executed from the context of the central event queue. */
static void sht31_driver_timer_expired_cb(void *user_data)
{
    if (sht3x_timer_data.cb) {
        sht3x_timer_data.cb(sht3x_timer_data.user_data);
    }
}

/**
 * @brief Map I2C result code to SHT3X I2C result code.
 *
 * @param i2c_rc I2C result code. One of @ref I2cResultCode.
 *
 * @return uint8_t Corresponding SHT3X I2C result code. One of @ref SHT3X_I2CResultCode.
 */
static uint8_t map_i2c_result_code_to_sht3x_i2c_result_code(uint8_t i2c_rc)
{
    uint8_t sht3x_i2c_rc;
    switch (i2c_rc) {
    case I2C_RESULT_CODE_OK:
        sht3x_i2c_rc = SHT3X_I2C_RESULT_CODE_OK;
        break;
    case I2C_RESULT_CODE_ERR:
        sht3x_i2c_rc = SHT3X_I2C_RESULT_CODE_BUS_ERROR;
        break;
    case I2C_RESULT_CODE_ADDR_NACK:
        sht3x_i2c_rc = SHT3X_I2C_RESULT_CODE_ADDRESS_NACK;
        break;
    default:
        /* Invalid I2C result code */
        EAS_ASSERT(0);
        break;
    }
    return sht3x_i2c_rc;
}

/**
 * @brief Handler for I2C transaction being complete.
 *
 * @param i2c_rc I2C result code. One of @ref I2cResultCode.
 */
static void handle_i2c_trans_complete(uint8_t i2c_rc)
{
    uint8_t sht3x_i2c_rc = map_i2c_result_code_to_sht3x_i2c_result_code(i2c_rc);
    if (sht3x_driver_i2c_data.cb) {
        sht3x_driver_i2c_data.cb(sht3x_i2c_rc, sht3x_driver_i2c_data.user_data);
    }
}

/**
 * @brief NRF TWIM event handler. Executed inside ISR context.
 *
 * @param p_event NRF TWIM event.
 * @param p_context User data.
 */
static void twim_handler(nrfx_twim_event_t const *p_event, void *p_context)
{
    uint8_t i2c_rc;
    switch (p_event->type) {
    case NRFX_TWIM_EVT_DONE:
        i2c_rc = I2C_RESULT_CODE_OK;
        break;
    case NRFX_TWIM_EVT_ADDRESS_NACK:
        i2c_rc = I2C_RESULT_CODE_ADDR_NACK;
        break;
    case NRFX_TWIM_EVT_DATA_NACK:
        i2c_rc = I2C_RESULT_CODE_ERR;
        break;
    case NRFX_TWIM_EVT_OVERRUN:
        i2c_rc = I2C_RESULT_CODE_ERR;
        break;
    case NRFX_TWIM_EVT_BUS_ERROR:
        i2c_rc = I2C_RESULT_CODE_ERR;
        break;
    default:
        /* Invalid TWIM event */
        EAS_ASSERT(0);
        break;
    }
    central_event_queue_submit_void_cb_with_uint8_event(handle_i2c_trans_complete, i2c_rc);
}

/**
 * @brief Initialize NRF TWIM driver - performs I2C transactions.
 */
static void init_nrfx_twim()
{
    IRQ_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_TWIM_INST_GET(TWIM_INST_IDX)), IRQ_PRIO_LOWEST, nrfx_twim_irq_handler,
                &twim_inst, 0);

    nrfx_twim_config_t twim_config = {
        .scl_pin = I2C_SCL_PIN,
        .sda_pin = I2C_SDA_PIN,
        .frequency = I2C_TWIM_FREQ,
        /* Taken from NRFX_TWIM_DEFAULT_CONFIG() */
        .interrupt_priority = NRFX_TWIM_DEFAULT_CONFIG_IRQ_PRIORITY,
        /* Taken from NRFX_TWIM_DEFAULT_CONFIG() */
        .hold_bus_uninit = false,
        /* Since we are not using i2c in Zephyr, gpios will be initialized by nrfx_twim driver */
        .skip_gpio_cfg = false,
        .skip_psel_cfg = false,
    };
    int rc = nrfx_twim_init(&twim_inst, &twim_config, twim_handler, NULL);
    EAS_ASSERT(rc == 0);
    nrfx_twim_enable(&twim_inst);
}

void hw_platform_init()
{
    init_nrfx_twim();
    sht31_driver_timer = eas_timer_create(0, EAS_TIMER_ONE_SHOT, sht31_driver_timer_expired_cb, NULL);

    SHT3XInitConfig sht3x_cfg = {
        .get_instance_memory = sht3x_driver_get_instance_memory,
        .get_instance_memory_user_data = NULL,
        .i2c_write = sht3x_driver_i2c_write,
        .i2c_write_user_data = (void *)&sht3x_driver_i2c_data,
        .i2c_read = sht3x_driver_i2c_read,
        .i2c_read_user_data = (void *)&sht3x_driver_i2c_data,
        .start_timer = sht3x_driver_timer_start,
        .start_timer_user_data = (void *)&sht3x_timer_data,
        .i2c_addr = SHT31_I2C_ADDR,
    };
    uint8_t rc = sht3x_create(&sht3x_inst, &sht3x_cfg);
    EAS_ASSERT(rc == SHT3X_RESULT_CODE_OK);

    SHT31VirtualInterfaces sht31_interfaces = virtual_sht31_initialize(&sht3x_inst);
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
