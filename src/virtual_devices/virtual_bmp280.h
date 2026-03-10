#ifndef ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_BMP280_H
#define ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_BMP280_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "pressure_sensor.h"
#include "bmp280.h"

/** All virtual devices that are based on BMP280. */
typedef struct BMP280VirtualInterfaces {
    const PressureSensor *const pressure_sensor;
} BMP280VirtualInterfaces;

/**
 * @brief Initialize virtual devices for the BMP280 implementation.
 *
 * @pre BMP280 hardware is initialized. It is in the state where functions from the @ref PressureSensor interface
 * can be called.
 *
 * @param bmp280_driver_inst Pointer to BMP280 driver instance.
 *
 * @return BMP280VirtualInterfaces Initialized virtual device interfaces that contain valid, ready-to-use function
 * pointers.
 */
BMP280VirtualInterfaces virtual_bmp280_initialize(BMP280 *const bmp280_driver_inst);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_BMP280_H */
