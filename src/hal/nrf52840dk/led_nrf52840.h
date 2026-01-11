#ifndef ENV_ALERT_SYSTEM_SRC_HAL_NRF52840DK_LED_NRF52840_H
#define ENV_ALERT_SYSTEM_SRC_HAL_NRF52840DK_LED_NRF52840_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "led.h"

/**
 * @brief Returns the LED interface with function pointers initialized to functions that implement the Led interface for
 * nrf52840 led.
 *
 * @return const Led* const nrf52840 LED interface.
 */
const Led *const led_nrf52840_get();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_HAL_NRF52840DK_LED_NRF52840_H */
