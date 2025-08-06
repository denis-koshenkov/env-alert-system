#ifndef PRESSURE_VALUE_H
#define PRESSURE_VALUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "hal/pressure.h"

/**
 * @brief Stores a pressure value and reports when it changes.
 */
typedef struct pressure_value_struct *pressure_value;

/**
 * @brief Create a pressure value instance.
 *
 * @return pressure_value Pressure value instance.
 */
pressure_value pressure_value_create();

/**
 * @brief Set pressure value.
 *
 * @param pv Pressure value instance returned by @ref pressure_value_create.
 * @param pressure Pressure value to set.
 */
void pressure_value_set(pressure_value pv, pressure pressure);

/**
 * @brief Get pressure value.
 *
 * @pre @ref pressure_value_set has been called at least once.
 *
 * @param pv Pressure value instance returned by @ref pressure_value_create.
 *
 * @return pressure Pressure value.
 */
pressure pressure_value_get(pressure_value pv);

/**
 * @brief Check whether the pressure value has changed.
 *
 * @pre @ref pressure_value_set has been called at least once.
 *
 * @param pv Pressure value instance returned by @ref pressure_value_create.
 *
 * @return true The last two calls to @ref pressure_value_set had the same value as arguments, or @ref
 * pressure_value_set has been called only once so far.
 * @return false The last two calls to @ref pressure_value_set had differing values as arguments.
 */
bool pressure_value_is_value_changed(pressure_value pv);

#ifdef __cplusplus
}
#endif

#endif
