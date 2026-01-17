#ifndef ENV_ALERT_SYSTEM_SRC_HAL_DEF_HW_PLATFORM_H
#define ENV_ALERT_SYSTEM_SRC_HAL_DEF_HW_PLATFORM_H

#include "led.h"
#include "temperature_sensor.h"
#include "pressure_sensor.h"
#include "humidity_sensor.h"
#include "light_intensity_sensor.h"

/**
 * @brief Hardware platform operation complete callback.
 *
 * @param user_data User data.
 */
typedef void (*HwPlatformCompleteCb)(void *user_data);

/**
 * @brief Initialize hardware platform.
 *
 * This function should be called on initialization. The application is only allowed to call the get functions to use
 * the virtual device interfaces after @p cb passed to this function is executed.
 *
 * The implementation should do everything necessary to fully initialize the virtual devices returned by the get
 * functions.
 *
 * @param cb Callback to execute once hardware initialization is complete.
 * @param user_data User data to pass to @p cb.
 */
void hw_platform_init(HwPlatformCompleteCb cb, void *user_data);

/**
 * @brief Get led instance from hardware platform.
 *
 * The implementation should return a led interface initialized with valid functions to perform led operations. The
 * application then uses the provided instance to control the led.
 *
 * @return const Led* const Led instance.
 */
const Led *const hw_platform_get_led();

/**
 * @brief Get temperature sensor instance from the hardware platform.
 *
 * The implementation should return a temperature sensor interface initialized with valid functions.
 *
 * @return const TemperatureSensor* const Temperature sensor instance.
 */
const TemperatureSensor *const hw_platform_get_temperature_sensor();

/**
 * @brief Get pressure sensor instance from the hardware platform.
 *
 * The implementation should return a pressure sensor interface initialized with valid functions.
 *
 * @return const PressureSensor* const Pressure sensor instance.
 */
const PressureSensor *const hw_platform_get_pressure_sensor();

/**
 * @brief Get humidity sensor instance from the hardware platform.
 *
 * The implementation should return a humidity sensor interface initialized with valid functions.
 *
 * @return const HumiditySensor* const Humidity sensor instance.
 */
const HumiditySensor *const hw_platform_get_humidity_sensor();

/**
 * @brief Get light intensity sensor instance from the hardware platform.
 *
 * The implementation should return a light intensity sensor interface initialized with valid functions.
 *
 * @return const LightIntensitySensor* const Light intensity sensor instance.
 */
const LightIntensitySensor *const hw_platform_get_light_intensity_sensor();

#endif /* ENV_ALERT_SYSTEM_SRC_HAL_DEF_HW_PLATFORM_H */
