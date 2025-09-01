#ifndef HAL_PRESSURE_H
#define HAL_PRESSURE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/**
 * @brief Pressure data type definition.
 *
 * Pressure is represented in hectopascals (hPa), with a precision of one decimal point. Since pressure in hPa cannot be
 * negtaive, an unsigned integer data type is used. Some examples of how various pressure values are represented:
 *   - 900 hPa -> 9000
 *   - 0 hPa -> 0
 *   - 1111 hPa -> 11110
 *   - 876.5 hPa -> 8765
 *
 * Pressure values range allowed by representation (in hPa): 0.0 to 6553.5.
 */
typedef uint16_t Pressure;

#ifdef __cplusplus
}
#endif

#endif
