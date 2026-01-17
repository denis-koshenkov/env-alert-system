#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC2_MOCKS_MOCK_LED_NOTIFICATION_EXECUTOR_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC2_MOCKS_MOCK_LED_NOTIFICATION_EXECUTOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "hal/led_defs.h"

void led_notification_executor_execute(LedColor led_color, LedPattern led_pattern, bool should_be_displayed);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC2_MOCKS_MOCK_LED_NOTIFICATION_EXECUTOR_H */
