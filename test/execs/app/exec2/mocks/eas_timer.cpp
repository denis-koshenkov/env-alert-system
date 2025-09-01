#include "CppUTestExt/MockSupport.h"
#include "eas_timer.h"
#include "eas_assert.h"

EasTimer eas_timer_create(uint32_t period_ms, EasTimerCb cb, void *user_data)
{
    EasTimerCb *timer_cbs = (EasTimerCb *)mock().getData("timerCbs").getPointerValue();
    void **timer_cbs_user_data = (void **)mock().getData("timerCbsUserData").getPointerValue();
    size_t num_timer_cbs = mock().getData("numTimerCbs").getUnsignedIntValue();

    static size_t timer_cbs_index = 0;
    EAS_ASSERT(timer_cbs_index < num_timer_cbs);
    timer_cbs[timer_cbs_index] = cb;
    timer_cbs_user_data[timer_cbs_index] = user_data;
    timer_cbs_index = (timer_cbs_index + 1) % num_timer_cbs;

    mock()
        .actualCall("eas_timer_create")
        .withParameter("period_ms", period_ms)
        .withParameter("cb", cb)
        .withParameter("user_data", user_data);
    return (EasTimer)mock().pointerReturnValue();
}

void eas_timer_set_period(EasTimer self, uint32_t period_ms)
{
    mock().actualCall("eas_timer_set_period").withParameter("self", self).withParameter("period_ms", period_ms);
}

void eas_timer_start(EasTimer self)
{
    mock().actualCall("eas_timer_start").withParameter("self", self);
}
