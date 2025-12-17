#include <stddef.h>

// TODO: remove
#include <zephyr/kernel.h>

#include "virtual_sht31.h"
#include "eas_assert.h"

static void temperature_register_new_sample_cb(TemperatureSensorNewSampleCb cb, void *user_data);
static void temperature_start();

static TemperatureSensor temperature_sensor = {
    .register_new_sample_cb = temperature_register_new_sample_cb,
    .start = temperature_start,
};

/* Temperature sensor private data */
TemperatureSensorNewSampleCb temperature_new_sample_cb = NULL;
void *temperature_new_sample_cb_user_data = NULL;
/* Temporary zephyr timer to generate samples. TODO: remove */
#define TEMPERATURE_SENSOR_TIMER_PERIOD_MS 1000

/* TODO: remove */
static void temperature_sensor_timer_expiry_function(struct k_timer *timer)
{
    if (temperature_new_sample_cb) {
        Temperature dummy_sample = 200; // 20.0 degrees Celsius
        temperature_new_sample_cb(dummy_sample, temperature_new_sample_cb_user_data);
    }
}
K_TIMER_DEFINE(temperature_sensor_timer, temperature_sensor_timer_expiry_function, NULL);

SHT31VirtualInterfaces virtual_sht31_initialize()
{
    return (SHT31VirtualInterfaces){&temperature_sensor};
}

static void temperature_register_new_sample_cb(TemperatureSensorNewSampleCb cb, void *user_data)
{
    EAS_ASSERT(cb);
    temperature_new_sample_cb = cb;
    temperature_new_sample_cb_user_data = user_data;
}

static void temperature_start()
{
    k_timer_start(&temperature_sensor_timer, K_MSEC(TEMPERATURE_SENSOR_TIMER_PERIOD_MS),
                  K_MSEC(TEMPERATURE_SENSOR_TIMER_PERIOD_MS));
}
