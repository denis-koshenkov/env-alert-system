#ifndef ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INTERFACES_HAL_VIRTUAL_DEVICES_VIRTUAL_BMP280_H
#define ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INTERFACES_HAL_VIRTUAL_DEVICES_VIRTUAL_BMP280_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "pressure_sensor.h"

/** All virtual devices that are based on BMP280. */
typedef struct BMP280VirtualInterfaces {
    const PressureSensor *const pressure_sensor;
} BMP280VirtualInterfaces;

/**
 * @brief Initialize virtual devices for the BMP280 implementation.
 *
 * @return BMP280VirtualInterfaces Initialized virtual device interfaces that contain valid, ready-to-use function
 * pointers.
 */
BMP280VirtualInterfaces virtual_bmp280_initialize();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INTERFACES_HAL_VIRTUAL_DEVICES_VIRTUAL_BMP280_H */
