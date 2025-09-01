#ifndef CURRENT_PRESSURE_H
#define CURRENT_PRESSURE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "hal/pressure.h"

/**
 * @brief This module stores current pressure and reports when it changes.
 */

/**
 * @brief Set current pressure value.
 *
 * @param pressure Pressure value to set.
 */
void current_pressure_set(Pressure pressure);

/**
 * @brief Get current pressure value.
 *
 * @pre @ref current_pressure_set has been called at least once.
 *
 * @return pressure Current pressure value.
 */
Pressure current_pressure_get();

/**
 * @brief Check whether the current pressure value has changed.
 *
 * @pre @ref current_pressure_set has been called at least once.
 *
 * @return true The last two calls to @ref current_pressure_set had the same value as arguments, or @ref
 * current_pressure_set has been called only once so far.
 * @return false The last two calls to @ref current_pressure_set had differing values as arguments.
 */
bool current_pressure_is_changed();

#ifdef __cplusplus
}
#endif

#endif
