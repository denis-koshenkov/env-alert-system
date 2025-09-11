#ifndef ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_EAS_TIMER_H
#define ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_EAS_TIMER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

#include "eas_timer_defs.h"

/**
 * @brief Environment alert system timer module.
 *
 * This is a software timer module that allows to schedule execution of a callback function in the future. It is also
 * possible to periodically execute a callback function using a periodic timer.
 */

/* Convenience macros for readability to pass to the "periodic" parameter of eas_timer_create */
#define EAS_TIMER_PERIODIC true
#define EAS_TIMER_ONE_SHOT false

/**
 * @brief Create a timer instance.
 *
 * Creates a timer that will execute @p cb with @p user_data after @p period_ms ms elapse since the timer is started.
 *
 * @param period_ms The amount of time that passes from the moment the timer is started to the moment the timer expiry
 * @p cb is executed.
 * @param periodic If true, creates a periodic timer, meaning that @p cb keeps being executed every @p period_ms ms. If
 * false, creates a one-shot timer, meaning that @p cb is executed only once.
 * @param cb Callback to execute when the timer expires. It is guaranteed that this callback will be executed from the
 * same context (thread) from which @ref eas_timer_create and @ref eas_timer_start were called to create and start the
 * timer.
 * @param user_data User data to pass to @p cb.
 *
 * @return EasTimer Timer instance.
 */
EasTimer eas_timer_create(uint32_t period_ms, bool periodic, EasTimerCb cb, void *user_data);

/**
 * @brief Set period for the timer.
 *
 * @param self Timer instance returned by @ref eas_timer_create.
 * @param period_ms New timer period in ms.
 *
 * @warning This function should only be called if the timer is currently not running. If this function is called when
 * the timer is running, behavior is undefined.
 */
void eas_timer_set_period(EasTimer self, uint32_t period_ms);

/**
 * @brief Start the timer.
 *
 * If the timer is already running, this will restart the timer. However, in this scenario it is possible that the timer
 * expiry callback will still be executed for the previous timer run.
 *
 * @param self Timer instance returned by @ref eas_timer_create.
 */
void eas_timer_start(EasTimer self);

/**
 * @brief Stop the timer.
 *
 * It is possible that the timer expiry callback will still be executed after this function is called.
 *
 * @param self Timer instance returned by @ref eas_timer_create.
 */
void eas_timer_stop(EasTimer self);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_EAS_TIMER_H */
