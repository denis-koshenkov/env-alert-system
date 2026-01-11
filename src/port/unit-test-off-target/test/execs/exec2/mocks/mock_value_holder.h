#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_APP_EXEC2_MOCKS_MOCK_VALUE_HOLDER_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_APP_EXEC2_MOCKS_MOCK_VALUE_HOLDER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>

typedef struct ValueHolderStruct *ValueHolder;

ValueHolder value_holder_create(uint8_t *value_buf, size_t value_size);

void value_holder_set(ValueHolder vh, const void *value);

void value_holder_get(ValueHolder vh, void *value);

bool value_holder_is_value_changed(ValueHolder vh);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_EXECS_APP_EXEC2_MOCKS_MOCK_VALUE_HOLDER_H */
