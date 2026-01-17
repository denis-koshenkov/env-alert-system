#ifndef ENV_ALERT_SYSTEM_SRC_APP_PRESSURE_VALUE_H
#define ENV_ALERT_SYSTEM_SRC_APP_PRESSURE_VALUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "hal/pressure.h"

/**
 * @brief Stores a pressure value and reports when it changes.
 */
typedef struct PressureValueStruct *PressureValue;

/**
 * @brief Create a pressure value instance.
 *
 * @return PressureValue Pressure value instance.
 */
PressureValue pressure_value_create();

/**
 * @brief Set pressure value.
 *
 * @param pv Pressure value instance returned by @ref pressure_value_create.
 * @param pressure Pressure value to set.
 */
void pressure_value_set(PressureValue pv, Pressure pressure);

/**
 * @brief Get pressure value.
 *
 * @pre @ref pressure_value_set has been called at least once.
 *
 * @param pv Pressure value instance returned by @ref pressure_value_create.
 *
 * @return pressure Pressure value.
 */
Pressure pressure_value_get(PressureValue pv);

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
bool pressure_value_is_value_changed(PressureValue pv);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_PRESSURE_VALUE_H */
