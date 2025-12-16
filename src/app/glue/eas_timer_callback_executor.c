#include "eas_timer_callback_executor.h"
#include "central_event_queue.h"

void eas_timer_callback_executor_execute_callback(EasTimerCb cb, void *user_data)
{
    /* EasTimerCb is a function type that returns void and has one parameter - void * user_data.
     * CentralEventQueueVoidCbWithUserData has the same signature, so we can safely cast. */
    central_event_queue_submit_void_cb_with_user_data_event((CentralEventQueueVoidCbWithUserData)cb, user_data);
}
