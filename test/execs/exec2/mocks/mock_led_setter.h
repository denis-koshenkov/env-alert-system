#ifndef ENV_ALERT_SYSTEM_TEST_EXECS_EXEC2_MOCKS_MOCK_LED_SETTER_H
#define ENV_ALERT_SYSTEM_TEST_EXECS_EXEC2_MOCKS_MOCK_LED_SETTER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "led_defs.h"

void led_setter_set(LedColor led_color, LedPattern led_pattern);

void led_setter_turn_off();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_TEST_EXECS_EXEC2_MOCKS_MOCK_LED_SETTER_H */
