#include <stddef.h>
#include <math.h>

#include "virtual_bmp280.h"
#include "eas_timer.h"
#include "eas_assert.h"
#include "bmp280.h"
#include "util.h"
#include "eas_log.h"

EAS_LOG_ENABLE_IN_FILE();

/* Implementation assumes that Pressure is uint16_t. If that changes, change the logic that checks for validity of
 * pressure value in convert_bmp280_pres_to_eas_pres. */
EAS_STATIC_ASSERT(sizeof(Pressure) == sizeof(uint16_t));

/* We set oversampling setting 1 for both temperature and pressure. Max measurement time for this combo is 6.4 ms
 * according to the datasheet, rounding it up to 7. */
#define BMP280_MEAS_TIME_MS 7

static void pressure_register_new_sample_cb(PressureSensorNewSampleCb cb, void *user_data);
static void pressure_start();

static PressureSensor pressure_sensor = {
    .register_new_sample_cb = pressure_register_new_sample_cb,
    .start = pressure_start,
};

static EasTimer bmp280_readout_timer;
static BMP280 *bmp280_inst_p;

static PressureSensorNewSampleCb pressure_new_sample_cb = NULL;
static void *pressure_new_sample_cb_user_data = NULL;

#define BMP280_READOUT_PERIOD_MS 1000

/**
 * @brief Execute a new pressure sample callback, if one is registered.
 *
 * @param[in] sample Pressure sample value to pass to the callback.
 */
static void execute_new_sample_cb(Pressure sample)
{
    if (pressure_new_sample_cb) {
        pressure_new_sample_cb(sample, pressure_new_sample_cb_user_data);
    }
}

/**
 * @brief Convert pressure value in BMP280 driver format to pressure value in EAS format.
 *
 * @param[in] bmp280_pres Pressure value in BMP280 driver format.
 * @param[out] eas_pres Pressure value in EAS format.
 *
 * @retval true Successfully converted pressure value.
 * @retval false Failed to convert - input value was out of bounds.
 */
static bool convert_bmp280_pres_to_eas_pres(uint32_t bmp280_pres, Pressure *const eas_pres)
{
    EAS_ASSERT(eas_pres);
    /* BMP280 pres is in Q24.8 format in Pa. Dividing by 2560 gives us a float value in hPa * 10. Round to get integer
     * value with the last digit being the first decimal point of value in hPa. */
    float eas_pres_f = roundf(bmp280_pres / 2560.0f);
    if (eas_pres_f < 0.0f || eas_pres_f > UINT16_MAX) {
        EAS_LOG_INF("Pressure value out of bounds");
        return false;
    }
    *eas_pres = (Pressure)eas_pres_f;
    return true;
}

/**
 * @brief BMP280 measurement complete callback.
 *
 * Converts pressure measurement to EAS format and executes the new sample callback, if one is registered.
 *
 * @param[in] rc BMP280 result code.
 * @param[in] user_data User data.
 */
static void bmp280_complete_cb(uint8_t rc, void *user_data)
{
    EAS_ASSERT(user_data);
    BMP280Meas *meas_p = (BMP280Meas *)user_data;

    if (rc != BMP280_RESULT_CODE_OK) {
        EAS_LOG_INF("Failed to read BMP280 meas");
        return;
    }

    Pressure eas_pres;
    bool converted = convert_bmp280_pres_to_eas_pres(meas_p->pressure, &eas_pres);
    if (!converted) {
        EAS_LOG_INF("Failed to convert BMP280 pressure to EAS pressure");
        return;
    }

    execute_new_sample_cb(eas_pres);
}

/**
 * @brief Initiates a BMP280 measurement in forced mode.
 *
 * @param user_data User data.
 */
static void bmp280_readout_timer_cb(void *user_data)
{
    /* Measurement results will be stored here. */
    static BMP280Meas meas;
    uint8_t rc = bmp280_read_meas_forced_mode(*bmp280_inst_p, BMP280_MEAS_TYPE_TEMP_AND_PRES, BMP280_MEAS_TIME_MS,
                                              &meas, bmp280_complete_cb, (void *)&meas);
    EAS_ASSERT(rc == BMP280_RESULT_CODE_OK);
}

BMP280VirtualInterfaces virtual_bmp280_initialize(BMP280 *const bmp280_driver_inst)
{
    EAS_ASSERT(bmp280_driver_inst);
    bmp280_inst_p = bmp280_driver_inst;
    bmp280_readout_timer =
        eas_timer_create(BMP280_READOUT_PERIOD_MS, EAS_TIMER_PERIODIC, bmp280_readout_timer_cb, NULL);
    return (BMP280VirtualInterfaces){&pressure_sensor};
}

static void pressure_register_new_sample_cb(PressureSensorNewSampleCb cb, void *user_data)
{
    EAS_ASSERT(cb);
    pressure_new_sample_cb = cb;
    pressure_new_sample_cb_user_data = user_data;
}

static void pressure_start()
{
    static bool is_started = false;
    EAS_ASSERT(!is_started);
    eas_timer_start(bmp280_readout_timer);
    is_started = true;
}
