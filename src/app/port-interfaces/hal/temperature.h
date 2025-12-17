#ifndef ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HAL_TEMPERATURE_H
#define ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HAL_TEMPERATURE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/**
 * @brief Temperature data type definition.
 *
 * Temperature is represented in Celsius, with a precision of one decimal point. Negative values are also allowed, hence
 * we use a signed data type to represent the temperature. Some examples of how various temperature values are
 * represented:
 *   - 20.0 degrees -> 200
 *   - 20.5 degrees -> 205
 *   - 1.9 degrees -> 19
 *   - 0.0 degrees -> 0
 *   - 0.1 degrees -> 1
 *   - -0.1 degrees -> -1
 *   - -1.0 degrees -> -10
 *   - -20.5 degrees -> -205
 *
 * Temperature values range allowed by representation (in Celsius): -3276.8 to 3276.7.
 */
typedef int16_t Temperature;

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HAL_TEMPERATURE_H */
