#ifndef ENV_ALERT_SYSTEM_SRC_APP_HUMIDITY_VALUE_H
#define ENV_ALERT_SYSTEM_SRC_APP_HUMIDITY_VALUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "humidity.h"

/**
 * @brief Stores a humidity value and reports when it changes.
 */
typedef struct HumidityValueStruct *HumidityValue;

/**
 * @brief Create a humidity value instance.
 *
 * @return HumidityValue Humidity value instance.
 */
HumidityValue humidity_value_create();

/**
 * @brief Set humidity value.
 *
 * @param hv Humidity value instance returned by @ref humidity_value_create.
 * @param humidity Humidity value to set.
 */
void humidity_value_set(HumidityValue hv, Humidity humidity);

/**
 * @brief Get humidity value.
 *
 * @pre @ref humidity_value_set has been called at least once.
 *
 * @param hv Humidity value instance returned by @ref humidity_value_create.
 *
 * @return humidity Humidity value.
 */
Humidity humidity_value_get(HumidityValue hv);

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
bool humidity_value_is_value_changed(HumidityValue hv);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_HUMIDITY_VALUE_H */
