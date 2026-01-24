#include <stddef.h>

#include "virtual_bh1750.h"
#include "eas_assert.h"
#include "eas_timer.h"
#include "eas_log.h"

EAS_LOG_ENABLE_IN_FILE();

#define BH1750_READOUT_PERIOD_MS 1000

static void light_intensity_register_new_sample_cb(LightIntensitySensorNewSampleCb cb, void *user_data);
static void light_intensity_start();

static LightIntensitySensor light_intensity_sensor = {
    .register_new_sample_cb = light_intensity_register_new_sample_cb,
    .start = light_intensity_start,
};

static EasTimer bh1750_readout_timer;
static BH1750 *bh1750_inst_p;
static uint32_t meas_lx;

static LightIntensitySensorNewSampleCb light_intensity_new_sample_cb = NULL;
static void *light_intensity_new_sample_cb_user_data = NULL;

/**
 * @brief Execute new light intensity sample callback if one is registered.
 *
 * @param sample Light intensity sample.
 */
static void execute_new_sample_cb(LightIntensity sample)
{
    if (light_intensity_new_sample_cb) {
        light_intensity_new_sample_cb(sample, light_intensity_new_sample_cb_user_data);
    }
}

/**
 * @brief Callback to execute once reading one time measurement from BH1750 is complete.
 *
 * @param result_code One of BH1750 result codes.
 * @param user_data Pointer to uint32_t which contains the measurement sample.
 */
static void one_time_meas_complete_cb(uint8_t result_code, void *user_data)
{
    if (result_code != BH1750_RESULT_CODE_OK) {
        EAS_LOG_INF("Failed to read BH1750 meas");
        return;
    }
    uint32_t *sample_p = (uint32_t *)user_data;
    EAS_ASSERT(sample_p);
    execute_new_sample_cb(*sample_p);
}

/* This callback will be executed from the central event queue context */
static void bh1750_readout_timer_cb(void *user_data)
{
    /* Pass &meas_lx as user data, so that we can pass the measurement sample to the new light intensity sample cb */
    uint8_t rc = bh1750_read_one_time_measurement(*bh1750_inst_p, BH1750_MEAS_MODE_H_RES, &meas_lx,
                                                  one_time_meas_complete_cb, &meas_lx);
    EAS_ASSERT(rc == BH1750_RESULT_CODE_OK);
}

BH1750VirtualInterfaces virtual_bh1750_initialize(BH1750 *const bh1750_driver_inst)
{
    EAS_ASSERT(bh1750_driver_inst);
    bh1750_inst_p = bh1750_driver_inst;

    bh1750_readout_timer =
        eas_timer_create(BH1750_READOUT_PERIOD_MS, EAS_TIMER_PERIODIC, bh1750_readout_timer_cb, NULL);
    return (BH1750VirtualInterfaces){&light_intensity_sensor};
}

static void light_intensity_register_new_sample_cb(LightIntensitySensorNewSampleCb cb, void *user_data)
{
    EAS_ASSERT(cb);
    light_intensity_new_sample_cb = cb;
    light_intensity_new_sample_cb_user_data = user_data;
}

static void light_intensity_start()
{
    static bool is_started = false;
    EAS_ASSERT(!is_started);
    eas_timer_start(bh1750_readout_timer);
    is_started = true;
}
