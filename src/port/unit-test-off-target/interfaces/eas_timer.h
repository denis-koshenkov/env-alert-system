#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_EAS_TIMER_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_EAS_TIMER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "eas_timer_defs.h"

/* Convenience macros for readability to pass to the "periodic" parameter of eas_timer_create */
#define EAS_TIMER_PERIODIC true
#define EAS_TIMER_ONE_SHOT false

EasTimer eas_timer_create(uint32_t period_ms, bool periodic, EasTimerCb cb, void *user_data);

void eas_timer_set_period(EasTimer self, uint32_t period_ms);

void eas_timer_start(EasTimer self);

void eas_timer_stop(EasTimer self);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_EAS_TIMER_H */
