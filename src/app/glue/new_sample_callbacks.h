#ifndef ENV_ALERT_SYSTEM_SRC_APP_GLUE_NEW_SAMPLE_CALLBACKS_H
#define ENV_ALERT_SYSTEM_SRC_APP_GLUE_NEW_SAMPLE_CALLBACKS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "hal/temperature.h"
#include "hal/pressure.h"
#include "hal/humidity.h"
#include "hal/light_intensity.h"

/**
 * @brief Callbacks to execute whenever a new sample is received from HAL.
 *
 * These functions are registered as callbacks with virtual devices that are exposed by the hardware platform.
 */

/**
 * @brief Callback to execute when a new temperature sample being received from the hardware.
 *
 * @param temperature Sample value.
 */
void new_sample_callback_temperature(Temperature sample, void *user_data);

/**
 * @brief Callback to execute when a new pressure sample being received from the hardware.
 *
 * @param pressure Sample value.
 */
void new_sample_callback_pressure(Pressure sample, void *user_data);

/**
 * @brief Callback to execute when a new humidity sample being received from the hardware.
 *
 * @param humidity Sample value.
 */
void new_sample_callback_humidity(Humidity sample, void *user_data);

/**
 * @brief Callback to execute when a new light intensity sample being received from the hardware.
 *
 * @param light_intensity Sample value.
 */
void new_sample_callback_light_intensity(LightIntensity sample, void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_GLUE_NEW_SAMPLE_CALLBACKS_H */
