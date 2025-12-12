#ifndef ENV_ALERT_SYSTEM_SRC_APP_GLUE_NEW_SAMPLE_HANDLER_H
#define ENV_ALERT_SYSTEM_SRC_APP_GLUE_NEW_SAMPLE_HANDLER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "hal/temperature.h"
#include "hal/pressure.h"
#include "hal/humidity.h"
#include "hal/light_intensity.h"

/**
 * @brief Callbacks to invoke whenever a new sample is received from HAL.
 */

/**
 * @brief Handles a new temperature sample being received from the hardware.
 *
 * @param temperature Sample value.
 */
void new_sample_handler_temperature(Temperature temperature);

/**
 * @brief Handles a new pressure sample being received from the hardware.
 *
 * @param pressure Sample value.
 */
void new_sample_handler_pressure(Pressure pressure);

/**
 * @brief Handles a new humidity sample being received from the hardware.
 *
 * @param humidity Sample value.
 */
void new_sample_handler_humidity(Humidity humidity);

/**
 * @brief Handles a new light intensity sample being received from the hardware.
 *
 * @param light_intensity Sample value.
 */
void new_sample_handler_light_intensity(LightIntensity light_intensity);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_GLUE_NEW_SAMPLE_HANDLER_H */
