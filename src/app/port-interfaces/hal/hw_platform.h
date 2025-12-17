#ifndef ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HAL_HW_PLATFORM_H
#define ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HAL_HW_PLATFORM_H

#include "led.h"
#include "temperature_sensor.h"
#include "humidity_sensor.h"

/**
 * @brief Initialize hardware platform.
 *
 * This function should be called on initialization. The application is only allowed to call the get functions to use
 * the virtual device interfaces after this function is called.
 *
 * The implementation should do everything necessary to fully initialize the virtual devices returned by the get
 * functions.
 */
void hw_platform_init();

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
 * @brief Get humidity sensor instance from the hardware platform.
 *
 * The implementation should return a humidity sensor interface initialized with valid functions.
 *
 * @return const HumiditySensor* const Humidity sensor instance.
 */
const HumiditySensor *const hw_platform_get_humidity_sensor();

#endif /* ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HAL_HW_PLATFORM_H */
