#include <stddef.h>

#include "alert_raiser.h"
#include "alert_notifier.h"
#include "eas_timer.h"
#include "eas_assert.h"
#include "config.h"

#ifndef CONFIG_ALERT_RAISER_MAX_NUM_INSTANCES
#define CONFIG_ALERT_RAISER_MAX_NUM_INSTANCES 1
#endif

struct AlertRaiserStruct {
    EasTimer warmup_timer;
    uint32_t warmup_period_ms;
    bool is_warmup_timer_running;
    EasTimer cooldown_timer;
    uint32_t cooldown_period_ms;
    bool is_cooldown_timer_running;
    uint8_t alert_id;
    bool is_alert_raised;
    bool is_alert_set;
};

static struct AlertRaiserStruct instances[CONFIG_ALERT_RAISER_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

static void warmup_period_expired_cb(void *user_data)
{
    AlertRaiser self = (AlertRaiser)(user_data);
    EAS_ASSERT(self);

    /* is_warmup_timer_running is set to false whenever the warmup timer is stopped. It could be the case that we
     * stopped the timer, but this callback still gets executed due to the timer implementation. If the timer is
     * stopped, its callback logic should not be executed. */
    if (self->is_warmup_timer_running) {
        alert_notifier_notify(self->alert_id, true);
        self->is_alert_raised = true;
        self->is_warmup_timer_running = false;
    }
}

static void cooldown_period_expired_cb(void *user_data)
{
    AlertRaiser self = (AlertRaiser)(user_data);
    EAS_ASSERT(self);

    /* is_cooldown_timer_running is set to false whenever the cooldown timer is stopped. It could be the case that we
     * stopped the timer, but this callback still gets executed due to the timer implementation. If the timer is
     * stopped, its callback logic should not be executed. */
    if (self->is_cooldown_timer_running) {
        alert_notifier_notify(self->alert_id, false);
        self->is_alert_raised = false;
        self->is_cooldown_timer_running = false;
    }
}

static void start_warmup_timer(AlertRaiser self)
{
    EAS_ASSERT(self);

    eas_timer_start(self->warmup_timer);
    self->is_warmup_timer_running = true;
}

static void start_cooldown_timer(AlertRaiser self)
{
    EAS_ASSERT(self);

    eas_timer_start(self->cooldown_timer);
    self->is_cooldown_timer_running = true;
}

static void stop_warmup_timer(AlertRaiser self)
{
    EAS_ASSERT(self);

    eas_timer_stop(self->warmup_timer);
    self->is_warmup_timer_running = false;
}

static void stop_cooldown_timer(AlertRaiser self)
{
    EAS_ASSERT(self);

    eas_timer_stop(self->cooldown_timer);
    self->is_cooldown_timer_running = false;
}

AlertRaiser alert_raiser_create()
{
    EAS_ASSERT(instance_idx < CONFIG_ALERT_RAISER_MAX_NUM_INSTANCES);
    struct AlertRaiserStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->warmup_timer = eas_timer_create(0, EAS_TIMER_ONE_SHOT, warmup_period_expired_cb, instance);
    instance->warmup_period_ms = 0;
    instance->is_warmup_timer_running = false;
    instance->cooldown_timer = eas_timer_create(0, EAS_TIMER_ONE_SHOT, cooldown_period_expired_cb, instance);
    instance->cooldown_period_ms = 0;
    instance->is_cooldown_timer_running = false;
    instance->alert_id = 0;
    instance->is_alert_raised = false;
    instance->is_alert_set = false;

    return instance;
}

void alert_raiser_set_alert(AlertRaiser self, uint8_t alert_id, uint32_t warmup_period_ms, uint32_t cooldown_period_ms)
{
    EAS_ASSERT(self);
    EAS_ASSERT(!self->is_alert_set);

    if (warmup_period_ms > 0) {
        eas_timer_set_period(self->warmup_timer, warmup_period_ms);
    }
    if (cooldown_period_ms > 0) {
        eas_timer_set_period(self->cooldown_timer, cooldown_period_ms);
    }

    self->warmup_period_ms = warmup_period_ms;
    self->cooldown_period_ms = cooldown_period_ms;
    self->alert_id = alert_id;
    self->is_alert_set = true;
}

void alert_raiser_unset_alert(AlertRaiser self)
{
    EAS_ASSERT(self);

    /* Stop any timers that are currently running */
    if (self->is_warmup_timer_running) {
        stop_warmup_timer(self);
    }
    if (self->is_cooldown_timer_running) {
        stop_cooldown_timer(self);
    }

    /* Silence the alert if it is currently raised */
    if (self->is_alert_raised) {
        alert_notifier_notify(self->alert_id, false);
        self->is_alert_raised = false;
    }

    self->is_alert_set = false;
}

bool alert_raiser_is_alert_set(AlertRaiser self)
{
    EAS_ASSERT(self);
    return self->is_alert_set;
}

void alert_raiser_set_alert_condition_result(AlertRaiser self, bool alert_condition_result)
{
    EAS_ASSERT(self);
    /* If an alert has not been set yet, we do not know for which alert this condition result is updated. An alert
     * should always be set prior to calling this function. */
    EAS_ASSERT(self->is_alert_set);

    bool already_in_required_state = (self->is_alert_raised == alert_condition_result);

    if (already_in_required_state) {
        /* We are already in the required state. The only thing to do is to stop the timer that would change the
         * state later, if such a timer is currently running. */

        /* If the alert is raised, only the cooldown timer can be running to silence the alert later. If the alert
         * is silenced, only the warmup timer can be running to raise the alert later. */
        bool is_timer_running = self->is_alert_raised ? self->is_cooldown_timer_running : self->is_warmup_timer_running;
        if (is_timer_running) {
            self->is_alert_raised ? stop_cooldown_timer(self) : stop_warmup_timer(self);
        }
    } else {
        uint32_t period_ms = alert_condition_result ? self->warmup_period_ms : self->cooldown_period_ms;
        if (period_ms > 0) {
            /* We are not in the required state and timer period is > 0. We need to ensure that there is a timer running
             * to change to the required state later. If there is no timer running, start it. If the timer is already
             * running, do nothing - this means that the previous call to this function had the same
             * alert_condition_result, and the timer had already been started. We should not restart the timer in that
             * case. */
            bool is_timer_running =
                alert_condition_result ? self->is_warmup_timer_running : self->is_cooldown_timer_running;
            if (!is_timer_running) {
                alert_condition_result ? start_warmup_timer(self) : start_cooldown_timer(self);
            }
        } else {
            /* We are not in the required state and the timer period is 0. Immediately change to the required state. */
            alert_notifier_notify(self->alert_id, alert_condition_result);
            self->is_alert_raised = alert_condition_result;
        }
    }
}
