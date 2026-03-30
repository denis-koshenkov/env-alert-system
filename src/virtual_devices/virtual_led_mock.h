#ifndef ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_LED_MOCK_H
#define ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_LED_MOCK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "led.h"

/**
 * @brief Returns the LED interface with function pointers initialized to functions that call the CppUTest mocking
 * framework.
 *
 * @return const Led* const Virtual LED mock interface.
 */
const Led *const virtual_led_mock_get();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_VIRTUAL_DEVICES_VIRTUAL_LED_MOCK_H */
