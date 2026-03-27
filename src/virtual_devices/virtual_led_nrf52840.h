#ifndef ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_LED_NRF52840_H
#define ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_LED_NRF52840_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "led.h"

typedef struct Nrf52840LedVirtualInterfaces {
    const Led *const led;
} Nrf52840LedVirtualInterfaces;

typedef struct {
    struct pwm_dt_spec red_pwm_led;
    struct pwm_dt_spec green_pwm_led;
    struct pwm_dt_spec blue_pwm_led;
} ZephyrPwms;

/**
 * @brief Initialize virtual device.
 *
 * @pre It has been verified that each of the pwm specs in @p zephyr_pwms is ready using pwm_is_ready_dt.
 *
 * @param[in] zephyr_pwms Zephyr PWM device tree specs. Must persist for the whole lifetime of this module - this module
 * accesses the pointer without making a local copy.
 *
 * @return Nrf52840LedVirtualInterfaces Virtual device interfaces based on Nrf52840 led.
 */
Nrf52840LedVirtualInterfaces virtual_led_nrf52840_initialize(const ZephyrPwms *const zephyr_pwms);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_LED_NRF52840_H */
