#include <stddef.h>

// TODO: remove
#include <zephyr/kernel.h>

#include "virtual_bmp280.h"
#include "eas_assert.h"

static void pressure_register_new_sample_cb(PressureSensorNewSampleCb cb, void *user_data);
static void pressure_start();

static PressureSensor pressure_sensor = {
    .register_new_sample_cb = pressure_register_new_sample_cb,
    .start = pressure_start,
};

PressureSensorNewSampleCb pressure_new_sample_cb = NULL;
void *pressure_new_sample_cb_user_data = NULL;

#define PRESSURE_SENSOR_TIMER_PERIOD_MS 1000

/* TODO: remove */
static void pressure_sensor_timer_expiry_function(struct k_timer *timer)
{
    if (pressure_new_sample_cb) {
        Pressure dummy_sample = 10150; // 1015.0 hPa
        pressure_new_sample_cb(dummy_sample, pressure_new_sample_cb_user_data);
    }
}
K_TIMER_DEFINE(pressure_sensor_timer, pressure_sensor_timer_expiry_function, NULL);

BMP280VirtualInterfaces virtual_bmp280_initialize()
{
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
    k_timer_start(&pressure_sensor_timer, K_MSEC(PRESSURE_SENSOR_TIMER_PERIOD_MS),
                  K_MSEC(PRESSURE_SENSOR_TIMER_PERIOD_MS));
}
