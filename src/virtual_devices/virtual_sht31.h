#ifndef ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_SHT31_H
#define ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_SHT31_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "temperature_sensor.h"
#include "humidity_sensor.h"
#include "sht3x.h"

/** All virtual devices that are based on SHT31. */
typedef struct SHT31VirtualInterfaces {
    const TemperatureSensor *const temperature_sensor;
    const HumiditySensor *const humidity_sensor;
} SHT31VirtualInterfaces;

/**
 * @brief Initialize virtual devices for the SHT31 implementation.
 *
 * @pre SHT31 hardware is initialized. It is in the state where functions from the @ref TemperatureSensor and @ref
 * HumiditySensor interfaces can be called.
 *
 * @param sht3x_driver_inst Pointer to SHT3X driver instance.
 *
 * @return SHT31VirtualInterfaces Initialized virtual device interfaces that contain valid, ready-to-use function
 * pointers.
 */
SHT31VirtualInterfaces virtual_sht31_initialize(SHT3X *const sht3x_driver_inst);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_SHT31_H */
