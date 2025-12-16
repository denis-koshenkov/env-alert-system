#ifndef ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_VARIABLE_REQUIREMENT_ALLOCATOR_H
#define ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_VARIABLE_REQUIREMENT_ALLOCATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

/** The implementation of this interface must be able to simulatensously allocate
 * CONFIG_VARIABLE_REQUIREMENT_ALLOCATOR_NUM_REQUIREMENTS memory blocks of size CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE
 * bytes. Each memory block is one variable requirement. Since the size of a variable requirement is not tied to a
 * specific data type, but is instead defined as a macro, this interface operates on void pointers.
 */

/**
 * @brief Allocate memory for a variable requirement.
 *
 * @return void* If successful, points to allocated memory for variable requirement. If failed due to being
 * out of memory, returns NULL.
 */
void *variable_requirement_allocator_alloc();

/**
 * @brief Free a previously allocated variable requirement.
 *
 * @param buf Variable requirement buffer previously returned by @ref variable_requirement_allocator_alloc.
 */
void variable_requirement_allocator_free(void *buf);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_VARIABLE_REQUIREMENT_ALLOCATOR_H */
