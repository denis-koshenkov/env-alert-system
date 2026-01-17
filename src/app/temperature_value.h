#ifndef ENV_ALERT_SYSTEM_SRC_APP_TEMPERATURE_VALUE_H
#define ENV_ALERT_SYSTEM_SRC_APP_TEMPERATURE_VALUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "hal/temperature.h"

/**
 * @brief Stores a temperature value and reports when it changes.
 */
typedef struct TemperatureValueStruct *TemperatureValue;

/**
 * @brief Create a temperature value instance.
 *
 * @return TemperatureValue Temperature value instance.
 */
TemperatureValue temperature_value_create();

/**
 * @brief Set temperature value.
 *
 * @param tv Temperature value instance returned by @ref temperature_value_create.
 * @param temperature Temperature value to set.
 */
void temperature_value_set(TemperatureValue tv, Temperature temperature);

/**
 * @brief Get temperature value.
 *
 * @pre @ref temperature_value_set has been called at least once.
 *
 * @param tv Temperature value instance returned by @ref temperature_value_create.
 *
 * @return temperature Temperature value.
 */
Temperature temperature_value_get(TemperatureValue tv);

/**
 * @brief Check whether the temperature value has changed.
 *
 * @pre @ref temperature_value_set has been called at least once.
 *
 * @param tv Temperature value instance returned by @ref temperature_value_create.
 *
 * @return true The last two calls to @ref temperature_value_set had the same value as arguments, or @ref
 * temperature_value_set has been called only once so far.
 * @return false The last two calls to @ref temperature_value_set had differing values as arguments.
 */
bool temperature_value_is_value_changed(TemperatureValue tv);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_TEMPERATURE_VALUE_H */
