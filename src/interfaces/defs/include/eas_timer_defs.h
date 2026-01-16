#ifndef ENV_ALERT_SYSTEM_SRC_APP_INTERFACES_PORT_EAS_TIMER_DEFS_H
#define ENV_ALERT_SYSTEM_SRC_APP_INTERFACES_PORT_EAS_TIMER_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Timer expiry callback.
 *
 * Executed when the timer period expires.
 *
 * @param user_data User data that was passed to @ref eas_timer_create function when this timer was created.
 */
typedef void (*EasTimerCb)(void *user_data);

typedef struct EasTimerStruct *EasTimer;

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_INTERFACES_PORT_EAS_TIMER_DEFS_H */
