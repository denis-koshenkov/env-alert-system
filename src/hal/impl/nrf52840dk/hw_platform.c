#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "nrfx_twim.h"

#include "hw_platform.h"
#include "hw_platform_private.h"
#include "virtual_led_nrf52840.h"
#include "eas_assert.h"
#include "virtual_sht31.h"
#include "virtual_bmp280.h"
#include "virtual_bh1750.h"
#include "virtual_transceiver_nrf_ble.h"
#include "central_event_queue.h"
#include "eas_timer.h"
#include "sht3x.h"
#include "sht3x/get_instance_memory.h"
#include "sht3x/interface.h"
#include "eas_log.h"
#include "ops_queue.h"

EAS_LOG_ENABLE_IN_FILE();

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

/* Maximum number of I2C operations that can be in the queue at the same time. We have two I2C sensors, and each of them
 * can do at most one I2C operation at the same time, so the queue size is 2 (one for each sensor). */
#define I2C_QUEUE_MAX_NUM_OPS 2
static OpsQueue i2c_queue;
static I2cOperation i2c_queue_ops_buf[I2C_QUEUE_MAX_NUM_OPS - 1];
static I2cOperation i2c_queue_op_buf;

/** Callback data for the I2C operation currently in progress. Once the I2C transaction is complete, this callback will
 * be executed. */
static I2cCompleteCbData i2c_complete_cb_data;

static SHT3X sht3x_inst;

/* SHT31 driver I2C related */
#define TWIM_INST_IDX 0
static nrfx_twim_t twim_inst = NRFX_TWIM_INSTANCE(NRF_TWIM_INST_GET(TWIM_INST_IDX));

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

/* When hw_platform_init is called, the user passes a callback to execute once hw init is complete. That callback and
 * its user data is stored here so that they can be invoked once hw platform init is complete.*/
static HwPlatformCompleteCb hw_init_complete_cb = NULL;
static void *hw_init_complete_cb_user_data = NULL;

/**
 * @brief Execute hw platform init complete callback, if one was provided.
 */
static void execute_hw_init_complete_cb()
{
    if (hw_init_complete_cb) {
        hw_init_complete_cb(hw_init_complete_cb_user_data);
    }
}

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
 * @brief Map I2C result code to BH1750 I2C result code.
 *
 * @param i2c_rc I2C result code. One of @ref I2cResultCode.
 *
 * @return uint8_t Corresponding BH1750 I2C result code. One of @ref BH1750_I2CResultCode.
 */
static uint8_t map_i2c_result_code_to_bh1750_i2c_result_code(uint8_t i2c_rc)
{
    uint8_t bh1750_i2c_rc;
    switch (i2c_rc) {
    case I2C_RESULT_CODE_OK:
        bh1750_i2c_rc = BH1750_I2C_RESULT_CODE_OK;
        break;
    case I2C_RESULT_CODE_ERR:
        bh1750_i2c_rc = BH1750_I2C_RESULT_CODE_ERR;
        break;
    case I2C_RESULT_CODE_ADDR_NACK:
        /* BH1750 is never supposed to NACK after address byte, so this counts as an error. */
        bh1750_i2c_rc = BH1750_I2C_RESULT_CODE_ERR;
        break;
    default:
        /* Invalid I2C result code */
        EAS_ASSERT(0);
        break;
    }
    return bh1750_i2c_rc;
}

/**
 * @brief Check whether I2C complete cb type is valid.
 *
 * @param cb_type I2C complete callback type, one of @ref I2cCompleteCbType.
 *
 * @retval true I2C complete cb type is valid.
 * @retval false I2C complete cb type is invalid.
 */
static bool is_valid_i2c_complete_cb_type(uint8_t cb_type)
{
    return ((cb_type == I2C_COMPLETE_CB_TYPE_SHT3X) || (cb_type == I2C_COMPLETE_CB_TYPE_BH1750));
}

/**
 * @brief Execute I2C complete callback.
 *
 * @param cb_data Data describing how the I2C callback should be executed (including the cb itself).
 * @param i2c_rc One of @ref I2cResultCode. I2C result code to execute the callback with. Depending on what type of
 * callback is being executed, this result code must be mapped to callback-specific I2C result code. For example, if
 * we are executing I2C complete callback for BH1750, this should be mapped to BH1750-specific I2C result code before
 * passing it to the callback as a parameter.
 */
static void execute_i2c_complete_cb(const I2cCompleteCbData *const cb_data, uint8_t i2c_rc)
{
    EAS_ASSERT(cb_data);
    EAS_ASSERT(is_valid_i2c_complete_cb_type(cb_data->cb_type));

    switch (cb_data->cb_type) {
    case I2C_COMPLETE_CB_TYPE_SHT3X: {
        uint8_t sht3x_i2c_rc = map_i2c_result_code_to_sht3x_i2c_result_code(i2c_rc);
        SHT3X_I2CTransactionCompleteCb cb = (SHT3X_I2CTransactionCompleteCb)cb_data->cb.sht3x_cb;
        if (cb) {
            cb(sht3x_i2c_rc, cb_data->user_data);
        }
        break;
    }
    case I2C_COMPLETE_CB_TYPE_BH1750: {
        uint8_t bh1750_i2c_rc = map_i2c_result_code_to_bh1750_i2c_result_code(i2c_rc);
        BH1750_I2CCompleteCb cb = (BH1750_I2CCompleteCb)cb_data->cb.bh1750_cb;
        if (cb) {
            cb(bh1750_i2c_rc, cb_data->user_data);
        }
        break;
    }
    default:
        /* Invalid i2c complete cb type */
        EAS_ASSERT(0);
        break;
    }
}

/**
 * @brief Handler for I2C transaction being complete.
 *
 * @param i2c_rc I2C result code. One of @ref I2cResultCode.
 */
static void handle_i2c_trans_complete(uint8_t i2c_rc)
{
    /* op_complete must be called before executing the I2C complete cb. op_complete will trigger the ops queue to start
     * the next I2C operation, if one is pending. This will free up a spot in the I2C queue. Executing i2c_complete_cb
     * might add a new operation to the I2C queue. Must be done in this order so that we are sure that there is a spot
     * in the queue in case i2c_complete_cb adds an operation to the queue.
     * The queue has size 2 - one slot for each I2C sensor. The assumption is that one sensor can never have more than
     * one I2C operation in the queue. Calling first op_complete and only then executing the i2c complete cb ensures
     * that that is indeed the case. */
    ops_queue_op_complete(i2c_queue);
    execute_i2c_complete_cb(&i2c_complete_cb_data, i2c_rc);
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
 * @brief I2C ops queue calls this function when it wants to start a I2C operation.
 *
 * Starts a I2C transaction using NRF TWIM driver.
 *
 * @param op I2C operation descriptor. Must point to data of type I2cOperation.
 * @param user_data User data.
 */
static void i2c_queue_start_op(void *op, void *user_data)
{
    I2cOperation *i2c_op_p = (I2cOperation *)op;
    nrfx_twim_t *twim_inst_p = (nrfx_twim_t *)user_data;
    EAS_ASSERT(i2c_op_p);
    EAS_ASSERT(twim_inst_p);

    /* Save the callback to execute to a local variable. Once the operation is complete, hw_platform will know that this
     * is the callback to execute. */
    memcpy(&i2c_complete_cb_data, &i2c_op_p->complete_cb_data, sizeof(I2cCompleteCbData));

    int rc = nrfx_twim_xfer(twim_inst_p, &i2c_op_p->twim_xfer_desc, 0);
    EAS_ASSERT(rc == 0);
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

/**
 * @brief Hardware platform initialization part 2.
 *
 * Performed after the SHT3X device was successfully reset.
 *
 * @param user_data This parameter is only here so that the function signature complies with what can be submitted to
 * event queue as a callback. It is not used in the implementation.
 */
static void hw_platform_init_part_2(void *user_data)
{
    SHT31VirtualInterfaces sht31_interfaces = virtual_sht31_initialize(&sht3x_inst);
    temperature_sensor = sht31_interfaces.temperature_sensor;
    humidity_sensor = sht31_interfaces.humidity_sensor;

    BMP280VirtualInterfaces bmp280_interfaces = virtual_bmp280_initialize();
    pressure_sensor = bmp280_interfaces.pressure_sensor;

    BH1750VirtualInterfaces bh1750_interfaces = virtual_bh1750_initialize();
    light_intensity_sensor = bh1750_interfaces.light_intensity_sensor;

    execute_hw_init_complete_cb();
    EAS_LOG_INF("Hw platform init complete");
}

/**
 * @brief Callback passed to SHT3X driver to be executed once the SHT3X device is reset.
 *
 * This submits part 2 of hw platform init to the central event queue as a callback, so that the rest of the hw platform
 * init is performed.
 *
 * @param result_code Signifies whether the SHT3X reset was successful.
 * @param user_data User data, unused in this callback.
 */
static void sht3x_reset_on_init_complete(uint8_t result_code, void *user_data)
{
    EAS_ASSERT(result_code == SHT3X_RESULT_CODE_OK);
    central_event_queue_submit_void_cb_with_user_data_event(hw_platform_init_part_2, NULL);
}

void hw_platform_init(HwPlatformCompleteCb cb, void *user_data)
{
    uint8_t rc;
    hw_init_complete_cb = cb;
    hw_init_complete_cb_user_data = user_data;

    init_nrfx_twim();
    sht31_driver_timer = eas_timer_create(0, EAS_TIMER_ONE_SHOT, sht31_driver_timer_expired_cb, NULL);
    /* Pass twim_inst as user data to i2c_queue_start_op - it uses the twim inst to start I2C transactions */
    i2c_queue = ops_queue_create(sizeof(I2cOperation), I2C_QUEUE_MAX_NUM_OPS, i2c_queue_ops_buf, &i2c_queue_op_buf,
                                 i2c_queue_start_op, &twim_inst);

    SHT3XInitConfig sht3x_cfg = {
        .get_instance_memory = sht3x_driver_get_instance_memory,
        .get_instance_memory_user_data = NULL,
        .i2c_write = sht3x_driver_i2c_write,
        .i2c_write_user_data = (void *)&i2c_queue,
        .i2c_read = sht3x_driver_i2c_read,
        .i2c_read_user_data = (void *)&i2c_queue,
        .start_timer = sht3x_driver_timer_start,
        .start_timer_user_data = (void *)&sht3x_timer_data,
        .i2c_addr = SHT31_I2C_ADDR,
    };
    rc = sht3x_create(&sht3x_inst, &sht3x_cfg);
    EAS_ASSERT(rc == SHT3X_RESULT_CODE_OK);

    /* Put SHT31 device into a known default state */
    rc = sht3x_soft_reset_with_delay(sht3x_inst, sht3x_reset_on_init_complete, NULL);
    EAS_ASSERT(rc == SHT3X_RESULT_CODE_OK);
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

const Transceiver *const hw_platform_get_transceiver()
{
    return virtual_transceiver_nrf_ble_get();
}
