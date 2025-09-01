#ifndef CURRENT_HUMIDITY_H
#define CURRENT_HUMIDITY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "hal/humidity.h"

/**
 * @brief This module stores current humidity and reports when it changes.
 */

/**
 * @brief Set current humidity value.
 *
 * @param humidity Humidity value to set.
 */
void current_humidity_set(Humidity humidity);

/**
 * @brief Get current humidity value.
 *
 * @pre @ref current_humidity_set has been called at least once.
 *
 * @return humidity Current humidity value.
 */
Humidity current_humidity_get();

/**
 * @brief Check whether the current humidity value has changed.
 *
 * @pre @ref current_humidity_set has been called at least once.
 *
 * @return true The last two calls to @ref current_humidity_set had the same value as arguments, or @ref
 * current_humidity_set has been called only once so far.
 * @return false The last two calls to @ref current_humidity_set had differing values as arguments.
 */
bool current_humidity_is_changed();

#ifdef __cplusplus
}
#endif

#endif
