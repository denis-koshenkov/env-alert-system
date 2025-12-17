#ifndef ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INTERFACES_HAL_VIRTUAL_DEVICES_VIRTUAL_SHT31_H
#define ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INTERFACES_HAL_VIRTUAL_DEVICES_VIRTUAL_SHT31_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "temperature.h"

/** All virtual devices that are based on SHT31. */
typedef struct SHT31VirtualInterfaces {
    const TemperatureSensor *const temperature_sensor;
} SHT31VirtualInterfaces;

/**
 * @brief Initialize virtual devices for the SHT31 implementation.
 *
 * @return SHT31VirtualInterfaces Initialized virtual device interfaces that contain valid, ready-to-use function
 * pointers.
 */
SHT31VirtualInterfaces virtual_sht31_initialize();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INTERFACES_HAL_VIRTUAL_DEVICES_VIRTUAL_SHT31_H */
