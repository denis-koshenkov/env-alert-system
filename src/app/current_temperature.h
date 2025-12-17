#ifndef ENV_ALERT_SYSTEM_SRC_APP_CURRENT_TEMPERATURE_H
#define ENV_ALERT_SYSTEM_SRC_APP_CURRENT_TEMPERATURE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "temperature.h"

/**
 * @brief This module stores current temperature and reports when it changes.
 */

/**
 * @brief Set current temperature value.
 *
 * @param temperature Temperature value to set.
 */
void current_temperature_set(Temperature temperature);

/**
 * @brief Get current temperature value.
 *
 * @pre @ref current_temperature_set has been called at least once.
 *
 * @return temperature Current temperature value.
 */
Temperature current_temperature_get();

/**
 * @brief Check whether the current temperature value has changed.
 *
 * @pre @ref current_temperature_set has been called at least once.
 *
 * @return true The last two calls to @ref current_temperature_set had the same value as arguments, or @ref
 * current_temperature_set has been called only once so far.
 * @return false The last two calls to @ref current_temperature_set had differing values as arguments.
 */
bool current_temperature_is_changed();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_CURRENT_TEMPERATURE_H */
