#ifndef MOCKS_EAS_TIMER_DEFS_H
#define MOCKS_EAS_TIMER_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*EasTimerCb)(void *user_data);

typedef struct EasTimerStruct *EasTimer;

#ifdef __cplusplus
}
#endif

#endif
