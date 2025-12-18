#include <stddef.h>

// TODO: remove
#include <zephyr/kernel.h>

#include "virtual_sht31.h"
#include "eas_assert.h"

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

/* Temperature sensor private data */
TemperatureSensorNewSampleCb temperature_new_sample_cb = NULL;
void *temperature_new_sample_cb_user_data = NULL;

/* Humidity sensor private data */
HumiditySensorNewSampleCb humidity_new_sample_cb = NULL;
void *humidity_new_sample_cb_user_data = NULL;

/* For temporary zephyr timers to generate samples. TODO: remove */
#define TEMPERATURE_SENSOR_TIMER_PERIOD_MS 1000
#define HUMIDITY_SENSOR_TIMER_PERIOD_MS 1000

/* TODO: remove */
static void temperature_sensor_timer_expiry_function(struct k_timer *timer)
{
    if (temperature_new_sample_cb) {
        Temperature dummy_sample = 200; // 20.0 degrees Celsius
        temperature_new_sample_cb(dummy_sample, temperature_new_sample_cb_user_data);
    }
}
K_TIMER_DEFINE(temperature_sensor_timer, temperature_sensor_timer_expiry_function, NULL);

/* TODO: remove */
static void humidity_sensor_timer_expiry_function(struct k_timer *timer)
{
    if (humidity_new_sample_cb) {
        Humidity dummy_sample = 800; // 80.0% RH
        humidity_new_sample_cb(dummy_sample, humidity_new_sample_cb_user_data);
    }
}
K_TIMER_DEFINE(humidity_sensor_timer, humidity_sensor_timer_expiry_function, NULL);

SHT31VirtualInterfaces virtual_sht31_initialize()
{
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
    k_timer_start(&temperature_sensor_timer, K_MSEC(TEMPERATURE_SENSOR_TIMER_PERIOD_MS),
                  K_MSEC(TEMPERATURE_SENSOR_TIMER_PERIOD_MS));
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
    k_timer_start(&humidity_sensor_timer, K_MSEC(HUMIDITY_SENSOR_TIMER_PERIOD_MS),
                  K_MSEC(HUMIDITY_SENSOR_TIMER_PERIOD_MS));
}
