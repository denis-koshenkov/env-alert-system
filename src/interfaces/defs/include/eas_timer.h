#ifndef ENV_ALERT_SYSTEM_SRC_INTERFACES_DEFS_INCLUDE_EAS_TIMER_H
#define ENV_ALERT_SYSTEM_SRC_INTERFACES_DEFS_INCLUDE_EAS_TIMER_H

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

/**
 * @brief A callback function prototype that executes the timer expiry callback.
 *
 * When a timer is created, an expiry callback function of type EasTimerCb is passed to @ref eas_timer_create. This
 * callback should be executed when the timer period expires.
 *
 * The implementation of eas_timer cannot invoke the EasTimerCb functions directly. The application module that created
 * and started the timer expects that the EasTimerCb callback is executed from the same thread/context as the
 * other functions of that application module.
 *
 * On the contrary, the implementation of this module likely relies on the timer implementation of the underlying RTOS.
 * In most cases, the RTOS timer implemenetation allows to execute a function after a certain period from ISR context or
 * from the context of a dedicated thread. This means that we cannot directly call EasTimerCb from that context.
 *
 * Instead, we call this EasTimerExecuteTimerExpiryFunctionCb callback from that context. To reiterate, it is allowed to
 * call this callback from any context - any thread or ISR. As parameters, this function gets the EasTimerCb and its
 * user_data that were passed to @ref eas_timer_create.
 *
 * The implementation of this callback is responsible for ensuring that the EasTimerCb is executed from the same
 * context/thread as the application module that is using the timer. An example implementation of this callback is to
 * submit an event to the event queue that handles all events in the system. As payload to that event, we pass the
 * EasTimerCb and user_data. Since the event queue handles all events in the system, the context from which EasTimer cb
 * is called is the same context from which all application module APIs are called.
 *
 * This is just an example implementation. Any other implementation is allowed, as long as EasTimerCb gets executed from
 * the same context as the application module that created and started that timer.
 *
 * The EasTimerExecuteTimerExpiryFunctionCb is set externally by calling @ref
 * eas_timer_set_execute_timer_expiry_function_cb. This callback is then used by ALL timer instances.
 */
typedef void (*EasTimerExecuteTimerExpiryFunctionCb)(EasTimerCb cb, void *user_data);

/* Convenience macros for readability to pass to the "periodic" parameter of eas_timer_create */
#define EAS_TIMER_PERIODIC true
#define EAS_TIMER_ONE_SHOT false

/**
 * @brief Set a callback that the eas_timer implementation will call when it wants to execute the timer expiry function.
 *
 * For details on the purpose of this function, see @ref EasTimerExecuteTimerExpiryFunctionCb.
 *
 * @param cb Callback to set.
 */
void eas_timer_set_execute_timer_expiry_function_cb(EasTimerExecuteTimerExpiryFunctionCb cb);

/**
 * @brief Create a timer instance.
 *
 * Creates a timer that will execute @p cb with @p user_data after @p period_ms ms elapse since the timer is started.
 *
 * @param period_ms The amount of time that passes from the moment the timer is started to the moment the timer expiry
 * @p cb is executed.
 * @param periodic If true, creates a periodic timer, meaning that @p cb keeps being executed every @p period_ms ms. If
 * false, creates a one-shot timer, meaning that @p cb is executed only once.
 * @param cb Callback to execute when the timer expires. When the timer expires, the execute timer expiry function will
 * be executed and @p cb and @p user_data will be passed to it. It is then the responsibility of the implementation of
 * that callback to invoke @p cb from the desired context/thread.
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
 * It is not allowed to call this function of period is set to 0. An assert is fired in that case.
 *
 * @param self Timer instance returned by @ref eas_timer_create.
 *
 * @warning An assert will fire if the timer expiry function cb has not been set yet. Set it by callng @ref
 * eas_timer_set_execute_timer_expiry_function_cb before starting any timers.
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

#endif /* ENV_ALERT_SYSTEM_SRC_INTERFACES_DEFS_INCLUDE_EAS_TIMER_H */
