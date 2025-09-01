#ifndef ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INTERFACES_VARIABLE_REQUIREMENT_ALLOCATOR_H
#define ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INTERFACES_VARIABLE_REQUIREMENT_ALLOCATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Abstract interface for the application to allocate memory for variable requirements.
 *
 * This module will implement zephyr's memory block allocator under the hood. Right now, it is defined as empty only to
 * avoid linker errors during the build.
 */

void *variable_requirement_allocator_alloc();

void variable_requirement_allocator_free(void *buf);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INTERFACES_VARIABLE_REQUIREMENT_ALLOCATOR_H */
