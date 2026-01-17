#include <stddef.h>
#include <math.h>

#include "virtual_sht31.h"
#include "eas_assert.h"
#include "eas_timer.h"
#include "eas_log.h"
#include "sht3x.h"
#include "util.h"

EAS_LOG_ENABLE_IN_FILE();

#define SHT31_TEMPERATURE_READOUT_PERIOD_MS 250
#define SHT31_HUMIDITY_READOUT_PERIOD_MS 250
EAS_STATIC_ASSERT(SHT31_TEMPERATURE_READOUT_PERIOD_MS == SHT31_HUMIDITY_READOUT_PERIOD_MS);

/* Temperature sensor section */
static void temperature_register_new_sample_cb(TemperatureSensorNewSampleCb cb, void *user_data);
static void temperature_start();

/* Humidity sensor section */
static void humidity_register_new_sample_cb(HumiditySensorNewSampleCb cb, void *user_data);
static void humidity_start();

static TemperatureSensor temperature_sensor = {
    .register_new_sample_cb = temperature_register_new_sample_cb,
    .start = temperature_start,
};

static HumiditySensor humidity_sensor = {
    .register_new_sample_cb = humidity_register_new_sample_cb,
    .start = humidity_start,
};

/* Common private data */
static EasTimer sht31_readout_timer;
static SHT3X *sht3x_inst;

/* Temperature sensor private data */
static TemperatureSensorNewSampleCb temperature_new_sample_cb = NULL;
static void *temperature_new_sample_cb_user_data = NULL;
static bool temperature_started = false;

/* Humidity sensor private data */
static HumiditySensorNewSampleCb humidity_new_sample_cb = NULL;
static void *humidity_new_sample_cb_user_data = NULL;
static bool humidity_started = false;

/**
 * @brief Execute temperature new sample callback, if one is registered.
 *
 * @param sample Temperature sample to pass to the callback.
 */
static void temperature_execute_new_sample_cb(Temperature sample)
{
    if (temperature_new_sample_cb) {
        temperature_new_sample_cb(sample, temperature_new_sample_cb_user_data);
    }
}

/**
 * @brief Execute humidity new sample callback, if one is registered.
 *
 * @param sample Humidity sample to pass to the callback.
 */
static void humidity_execute_new_sample_cb(Humidity sample)
{
    if (humidity_new_sample_cb) {
        humidity_new_sample_cb(sample, humidity_new_sample_cb_user_data);
    }
}

static void sht3x_meas_complete_cb(uint8_t result_code, SHT3XMeasurement *meas, void *user_data)
{
    if (result_code == SHT3X_RESULT_CODE_CRC_MISMATCH) {
        EAS_LOG_INF("Temperature or humidity CRC mismatch");
        return;
    } else if (result_code != SHT3X_RESULT_CODE_OK) {
        EAS_LOG_INF("Failed to read SHT3X meas");
        return;
    }
    EAS_ASSERT(meas);

    if (temperature_started) {
        /* One decimal point precision */
        Temperature temperature = lroundf(meas->temperature * 10.0f);
        temperature_execute_new_sample_cb(temperature);
    }
    if (humidity_started) {
        /* One decimal point precision */
        Humidity humidity = lroundf(meas->humidity * 10.0f);
        humidity_execute_new_sample_cb(humidity);
    }
}

/* This callback will be executed from the central event queue context */
static void sht31_readout_timer_cb(void *user_data)
{
    /* Read both temperature and humidity and verify their CRCs */
    static const uint8_t flags =
        SHT3X_FLAG_READ_TEMP | SHT3X_FLAG_READ_HUM | SHT3X_FLAG_VERIFY_CRC_TEMP | SHT3X_FLAG_VERIFY_CRC_HUM;
    uint8_t rc =
        sht3x_read_single_shot_measurement(*sht3x_inst, SHT3X_MEAS_REPEATABILITY_HIGH, SHT3X_CLOCK_STRETCHING_DISABLED,
                                           flags, sht3x_meas_complete_cb, NULL);
    EAS_ASSERT(rc == SHT3X_RESULT_CODE_OK);
}

SHT31VirtualInterfaces virtual_sht31_initialize(SHT3X *const sht3x_driver_inst)
{
    EAS_ASSERT(sht3x_driver_inst);
    sht3x_inst = sht3x_driver_inst;

    /* Using temperature readout period, assuming that it is the same as humidity readout period - there should be a
     * static assert verifying this. */
    sht31_readout_timer =
        eas_timer_create(SHT31_TEMPERATURE_READOUT_PERIOD_MS, EAS_TIMER_PERIODIC, sht31_readout_timer_cb, NULL);
    return (SHT31VirtualInterfaces){&temperature_sensor, &humidity_sensor};
}

/* Virtual temperature sensor functions */

static void temperature_register_new_sample_cb(TemperatureSensorNewSampleCb cb, void *user_data)
{
    EAS_ASSERT(cb);
    temperature_new_sample_cb = cb;
    temperature_new_sample_cb_user_data = user_data;
}

static void temperature_start()
{
    EAS_ASSERT(!temperature_started);
    if (!temperature_started && !humidity_started) {
        eas_timer_start(sht31_readout_timer);
    }
    temperature_started = true;
}

/* Virtual humidity sensor functions */

static void humidity_register_new_sample_cb(HumiditySensorNewSampleCb cb, void *user_data)
{
    EAS_ASSERT(cb);
    humidity_new_sample_cb = cb;
    humidity_new_sample_cb_user_data = user_data;
}

static void humidity_start()
{
    EAS_ASSERT(!humidity_started);
    if (!temperature_started && !humidity_started) {
        eas_timer_start(sht31_readout_timer);
    }
    humidity_started = true;
}
