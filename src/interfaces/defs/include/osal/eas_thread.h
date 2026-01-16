#ifndef ENV_ALERT_SYSTEM_SRC_INTERFACES_DEFS_INCLUDE_OSAL_EAS_THREAD_H
#define ENV_ALERT_SYSTEM_SRC_INTERFACES_DEFS_INCLUDE_OSAL_EAS_THREAD_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Thread entry function.
 */
typedef void (*EasThreadRunFunction)();

/**
 * @brief Create a thread with the given run function and start the thread immediately.
 *
 * @param run Thread entry function.
 */
void eas_thread_create(EasThreadRunFunction run_function);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_INTERFACES_DEFS_INCLUDE_OSAL_EAS_THREAD_H */
