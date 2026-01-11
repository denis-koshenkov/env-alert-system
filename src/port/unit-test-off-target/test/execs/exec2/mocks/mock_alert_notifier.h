#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC2_MOCKS_MOCK_ALERT_NOTIFIER_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC2_MOCKS_MOCK_ALERT_NOTIFIER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

void alert_notifier_notify(uint8_t alert_id, bool is_raised);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_EXEC2_MOCKS_MOCK_ALERT_NOTIFIER_H */
