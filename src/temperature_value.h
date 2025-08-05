#ifndef TEMPERATURE_VALUE_H
#define TEMPERATURE_VALUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "hal/temperature.h"

/**
 * @brief Stores a temperature value and reports when it changes.
 */
typedef struct temperature_value_struct *temperature_value;

/**
 * @brief Create a temperature value instance.
 *
 * @return temperature_value Temperature value instance.
 */
temperature_value temperature_value_create();

/**
 * @brief Set temperature value.
 *
 * @param tv Temperature value instance returned by @ref temperature_value_create.
 * @param temperature Temperature value to set.
 */
void temperature_value_set(temperature_value tv, temperature temperature);

/**
 * @brief Get temperature value.
 *
 * @pre @ref temperature_value_set has been called at least once.
 *
 * @param tv Temperature value instance returned by @ref temperature_value_create.
 *
 * @return temperature Temperature value.
 */
temperature temperature_value_get(temperature_value tv);

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
bool temperature_value_is_value_changed(temperature_value tv);

#ifdef __cplusplus
}
#endif

#endif
