#ifndef HUMIDITY_VALUE_H
#define HUMIDITY_VALUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "hal/humidity.h"

/**
 * @brief Stores a humidity value and reports when it changes.
 */
typedef struct humidity_value_struct *humidity_value;

/**
 * @brief Create a humidity value instance.
 *
 * @return humidity_value Humidity value instance.
 */
humidity_value humidity_value_create();

/**
 * @brief Set humidity value.
 *
 * @param hv Humidity value instance returned by @ref humidity_value_create.
 * @param humidity Humidity value to set.
 */
void humidity_value_set(humidity_value hv, humidity humidity);

/**
 * @brief Get humidity value.
 *
 * @pre @ref humidity_value_set has been called at least once.
 *
 * @param hv Humidity value instance returned by @ref humidity_value_create.
 *
 * @return humidity Humidity value.
 */
humidity humidity_value_get(humidity_value hv);

/**
 * @brief Check whether the humidity value has changed.
 *
 * @pre @ref humidity_value_set has been called at least once.
 *
 * @param hv Humidity value instance returned by @ref humidity_value_create.
 *
 * @return true The last two calls to @ref humidity_value_set had the same value as arguments, or @ref
 * humidity_value_set has been called only once so far.
 * @return false The last two calls to @ref humidity_value_set had differing values as arguments.
 */
bool humidity_value_is_value_changed(humidity_value hv);

#ifdef __cplusplus
}
#endif

#endif
