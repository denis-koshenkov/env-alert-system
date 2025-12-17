#ifndef ENV_ALERT_SYSTEM_SRC_APP_CURRENT_LIGHT_INTENSITY_H
#define ENV_ALERT_SYSTEM_SRC_APP_CURRENT_LIGHT_INTENSITY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "light_intensity.h"

/**
 * @brief This module stores current light intensity and reports when it changes.
 */

/**
 * @brief Set current light intensity value.
 *
 * @param light_intensity Light intensity value to set.
 */
void current_light_intensity_set(LightIntensity light_intensity);

/**
 * @brief Get current light intensity value.
 *
 * @pre @ref current_light_intensity_set has been called at least once.
 *
 * @return light_intensity Current light intensity value.
 */
LightIntensity current_light_intensity_get();

/**
 * @brief Check whether the current light intensity value has changed.
 *
 * @pre @ref current_light_intensity_set has been called at least once.
 *
 * @return true The last two calls to @ref current_light_intensity_set had the same value as arguments, or @ref
 * current_light_intensity_set has been called only once so far.
 * @return false The last two calls to @ref current_light_intensity_set had differing values as arguments.
 */
bool current_light_intensity_is_changed();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_CURRENT_LIGHT_INTENSITY_H */
