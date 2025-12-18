#include <stddef.h>

// TODO: remove
#include <zephyr/kernel.h>

#include "virtual_bh1750.h"
#include "eas_assert.h"

static void light_intensity_register_new_sample_cb(LightIntensitySensorNewSampleCb cb, void *user_data);
static void light_intensity_start();

static LightIntensitySensor light_intensity_sensor = {
    .register_new_sample_cb = light_intensity_register_new_sample_cb,
    .start = light_intensity_start,
};

LightIntensitySensorNewSampleCb light_intensity_new_sample_cb = NULL;
void *light_intensity_new_sample_cb_user_data = NULL;

#define LIGHT_INTENSITY_SENSOR_TIMER_PERIOD_MS 1000

/* TODO: remove */
static void light_intensity_sensor_timer_expiry_function(struct k_timer *timer)
{
    if (light_intensity_new_sample_cb) {
        LightIntensity dummy_sample = 1; // 1 lx
        light_intensity_new_sample_cb(dummy_sample, light_intensity_new_sample_cb_user_data);
    }
}
K_TIMER_DEFINE(light_intensity_sensor_timer, light_intensity_sensor_timer_expiry_function, NULL);

BH1750VirtualInterfaces virtual_bh1750_initialize()
{
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
    k_timer_start(&light_intensity_sensor_timer, K_MSEC(LIGHT_INTENSITY_SENSOR_TIMER_PERIOD_MS),
                  K_MSEC(LIGHT_INTENSITY_SENSOR_TIMER_PERIOD_MS));
}
