#ifndef ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_BH1750_H
#define ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_BH1750_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "light_intensity_sensor.h"
#include "bh1750.h"

/** All virtual devices that are based on BH1750. */
typedef struct BH1750VirtualInterfaces {
    const LightIntensitySensor *const light_intensity_sensor;
} BH1750VirtualInterfaces;

/**
 * @brief Initialize virtual devices for the BH1750 implementation.
 *
 * @pre BH1750 hardware is initialized. It is in the state where functions from the @ref LightIntensitySensor interface
 * can be called.
 *
 * @param bh1750_driver_inst Pointer to BH1750 driver instance.
 *
 * @return BH1750VirtualInterfaces Initialized virtual device interfaces that contain valid, ready-to-use function
 * pointers.
 */
BH1750VirtualInterfaces virtual_bh1750_initialize(BH1750 *const bh1750_driver_inst);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_BH1750_H */
