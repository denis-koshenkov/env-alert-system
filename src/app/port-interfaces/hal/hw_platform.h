#ifndef ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HAL_HW_PLATFORM_H
#define ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HAL_HW_PLATFORM_H

#include "led.h"

/**
 * @brief Get led instance from hardware platform.
 *
 * The implementation should return a led interface initialized with valid functions to perform led operations. The
 * application then uses the provided instance to control the led.
 *
 * @return const Led* Led instance.
 */
const Led *hw_platform_get_led();

#endif /* ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HAL_HW_PLATFORM_H */
