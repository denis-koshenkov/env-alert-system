#ifndef ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HAL_LIGHT_INTENSITY_H
#define ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HAL_LIGHT_INTENSITY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/**
 * @brief Light intensity data type definition.
 *
 * Light intensity is represented in whole lux (lx). Decimal point precision is not necessary for our application. Since
 * light intensity in lux cannot be negtaive, an unsigned integer data type is used. Some examples of how various light
 * intensity values are represented:
 *   - 0 lx → 0
 *   - 100 lx → 100
 *   - 12345 lx → 12345
 *   - 100000 lx → 100000
 *
 * Light intensity values range allowed by representation (in lx): 0 to 4294967295.
 */
typedef uint32_t LightIntensity;

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HAL_LIGHT_INTENSITY_H */
