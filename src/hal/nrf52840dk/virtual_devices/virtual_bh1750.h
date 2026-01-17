#ifndef ENV_ALERT_SYSTEM_SRC_HAL_NRF52840DK_VIRTUAL_DEVICES_VIRTUAL_BH1750_H
#define ENV_ALERT_SYSTEM_SRC_HAL_NRF52840DK_VIRTUAL_DEVICES_VIRTUAL_BH1750_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "hal/light_intensity_sensor.h"

/** All virtual devices that are based on BH1750. */
typedef struct BH1750VirtualInterfaces {
    const LightIntensitySensor *const light_intensity_sensor;
} BH1750VirtualInterfaces;

/**
 * @brief Initialize virtual devices for the BH1750 implementation.
 *
 * @return BH1750VirtualInterfaces Initialized virtual device interfaces that contain valid, ready-to-use function
 * pointers.
 */
BH1750VirtualInterfaces virtual_bh1750_initialize();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_HAL_NRF52840DK_VIRTUAL_DEVICES_VIRTUAL_BH1750_H */
