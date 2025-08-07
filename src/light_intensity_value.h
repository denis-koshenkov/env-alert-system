#ifndef LIGHT_INTENSITY_VALUE_H
#define LIGHT_INTENSITY_VALUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "hal/light_intensity.h"

/**
 * @brief Stores a light_intensity value and reports when it changes.
 */
typedef struct light_intensity_value_struct *light_intensity_value;

/**
 * @brief Create a light_intensity value instance.
 *
 * @return light_intensity_value Light intensity value instance.
 */
light_intensity_value light_intensity_value_create();

/**
 * @brief Set light_intensity value.
 *
 * @param liv Light intensity value instance returned by @ref light_intensity_value_create.
 * @param light_intensity Light intensity value to set.
 */
void light_intensity_value_set(light_intensity_value liv, light_intensity light_intensity);

/**
 * @brief Get light_intensity value.
 *
 * @pre @ref light_intensity_value_set has been called at least once.
 *
 * @param liv Light intensity value instance returned by @ref light_intensity_value_create.
 *
 * @return light_intensity Light intensity value.
 */
light_intensity light_intensity_value_get(light_intensity_value liv);

/**
 * @brief Check whether the light_intensity value has changed.
 *
 * @pre @ref light_intensity_value_set has been called at least once.
 *
 * @param liv Light intensity value instance returned by @ref light_intensity_value_create.
 *
 * @return true The last two calls to @ref light_intensity_value_set had the same value as arguments, or @ref
 * light_intensity_value_set has been called only once so far.
 * @return false The last two calls to @ref light_intensity_value_set had differing values as arguments.
 */
bool light_intensity_value_is_value_changed(light_intensity_value liv);

#ifdef __cplusplus
}
#endif

#endif
