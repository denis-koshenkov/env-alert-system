#ifndef ENV_ALERT_SYSTEM_SRC_APP_GLUE_EAS_TIMER_CALLBACK_EXECUTOR_H
#define ENV_ALERT_SYSTEM_SRC_APP_GLUE_EAS_TIMER_CALLBACK_EXECUTOR_H

#include "eas_timer_defs.h"

/**
 * @brief Execute a EasTimer callback function.
 *
 * This function exists to be passed to the EasTimer module as the "execute timer expiry function" callback. EasTimer
 * module calls this callback whenever it needs to execute a timer expiry function. EasTimer can invoke this callback
 * from any context (thread/ISR). The implementation of this function ensures that @p cb gets executed from the right
 * context/thread. Our application has one central event queue with a dedicated thread - this is the context from which
 * all application logic is executed. @p cb is executed from this context as well.
 *
 * @param cb Callback to execute.
 * @param user_data User data to pass to the callback.
 */
void eas_timer_callback_executor_execute_callback(EasTimerCb cb, void *user_data);

#endif /* ENV_ALERT_SYSTEM_SRC_APP_GLUE_EAS_TIMER_CALLBACK_EXECUTOR_H */
