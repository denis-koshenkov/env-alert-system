#ifndef ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_EAS_TIMER_DEFS_H
#define ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_EAS_TIMER_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*EasTimerCb)(void *user_data);

typedef struct EasTimerStruct *EasTimer;

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_EAS_TIMER_DEFS_H */
