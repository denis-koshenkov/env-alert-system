#include <zephyr/kernel.h>

#include "eas_timer.h"
#include "config.h"
#include "eas_assert.h"

#ifndef CONFIG_EAS_TIMER_MAX_NUM_INSTANCES
#define CONFIG_EAS_TIMER_MAX_NUM_INSTANCES 1
#endif

struct EasTimerStruct {
    struct k_timer timer;
    uint32_t period_ms;
    bool periodic;
    EasTimerCb cb;
    void *user_data;
};

static struct EasTimerStruct instances[CONFIG_EAS_TIMER_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

static EasTimerExecuteTimerExpiryFunctionCb execute_timer_expiry_function_cb = NULL;

/**
 * @brief This function serves as expiry function for every zephyr timer.
 *
 * @param timer Address of zephyr timer which is a part of our EasTimerStruct.
 *
 * @note This function is executed from ISR context.
 */
static void zephyr_timer_expiry_function(struct k_timer *timer)
{
    const struct EasTimerStruct *const self = CONTAINER_OF(timer, struct EasTimerStruct, timer);
    EAS_ASSERT(self);
    EAS_ASSERT(self->cb);
    EAS_ASSERT(execute_timer_expiry_function_cb);
    execute_timer_expiry_function_cb(self->cb, self->user_data);
}

void eas_timer_set_execute_timer_expiry_function_cb(EasTimerExecuteTimerExpiryFunctionCb cb)
{
    EAS_ASSERT(cb);
    execute_timer_expiry_function_cb = cb;
}

EasTimer eas_timer_create(uint32_t period_ms, bool periodic, EasTimerCb cb, void *user_data)
{
    /* The only way to notify the caller about timer expiry is via cb, so it does not make sense to create a timer
     * without a defined cb. */
    EAS_ASSERT(cb);

    EAS_ASSERT(instance_idx < CONFIG_EAS_TIMER_MAX_NUM_INSTANCES);
    struct EasTimerStruct *instance = &instances[instance_idx];
    instance_idx++;

    k_timer_init(&(instance->timer), zephyr_timer_expiry_function, NULL);
    instance->period_ms = period_ms;
    instance->periodic = periodic;
    instance->cb = cb;
    instance->user_data = user_data;
    return instance;
}

void eas_timer_set_period(EasTimer self, uint32_t period_ms)
{
    EAS_ASSERT(self);
    self->period_ms = period_ms;
}

void eas_timer_start(EasTimer self)
{
    /* Starting a timer is only allowed after the execute_timer_expiry_function_cb has been set, because
     * execute_timer_expiry_function_cb will be called as a part of the timer's expiry callback. */
    EAS_ASSERT(execute_timer_expiry_function_cb);
    EAS_ASSERT(self);
    /* This timer API is only for scheduling functions to execute in the future */
    EAS_ASSERT(self->period_ms != 0);

    k_timeout_t period = self->periodic ? K_MSEC(self->period_ms) : K_FOREVER;
    k_timer_start(&(self->timer), K_MSEC(self->period_ms), period);
}

void eas_timer_stop(EasTimer self)
{
    EAS_ASSERT(self);
    k_timer_stop(&(self->timer));
}
