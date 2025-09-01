#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_VARIABLE_REQUIREMENT_ALLOCATOR_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_VARIABLE_REQUIREMENT_ALLOCATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

void *variable_requirement_allocator_alloc();

void variable_requirement_allocator_free(void *buf);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_VARIABLE_REQUIREMENT_ALLOCATOR_H */
