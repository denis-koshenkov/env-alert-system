#ifndef ENV_ALERT_SYSTEM_SRC_HAL_DEF_HUMIDITY_H
#define ENV_ALERT_SYSTEM_SRC_HAL_DEF_HUMIDITY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/**
 * @brief Humidity data type definition.
 *
 * Humidity is represented as Relative Humidity (RH) in percentage, with a precision of one decimal point. Since
 * relative humidity in percentage cannot be negtaive, an unsigned integer data type is used. Some examples of how
 * various humidity values are represented:
 *   - 0.0% RH → 0
 *   - 10.1% RH → 101
 *   - 100.0% RH → 1000
 *   - 95.5% RH → 955
 *
 * The allowed range of values is 0 <= humidity <= 1000 which corresponds to 0.0% to 100.0%. Percentage points can only
 * be between 0 and 100.
 */
typedef uint16_t Humidity;

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_HAL_DEF_HUMIDITY_H */
