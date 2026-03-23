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

Nrf52840LedVirtualInterfaces virtual_led_nrf52840_initialize();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_LED_NRF52840_H */
