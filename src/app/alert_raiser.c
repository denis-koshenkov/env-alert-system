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
    bool is_raised;
};

static struct AlertRaiserStruct instances[CONFIG_ALERT_RAISER_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

static void warmup_period_expired_cb(void *user_data)
{
    AlertRaiser self = (AlertRaiser)(user_data);

    alert_notifier_notify(self->alert_id, true);
    self->is_raised = true;
    self->is_warmup_timer_running = false;
}

static void cooldown_period_expired_cb(void *user_data)
{
    AlertRaiser self = (AlertRaiser)(user_data);

    alert_notifier_notify(self->alert_id, false);
    self->is_raised = false;
    self->is_cooldown_timer_running = false;
}

static void start_warmup_timer(AlertRaiser self)
{
    eas_timer_start(self->warmup_timer);
    self->is_warmup_timer_running = true;
}

static void start_cooldown_timer(AlertRaiser self)
{
    eas_timer_start(self->cooldown_timer);
    self->is_cooldown_timer_running = true;
}

static void stop_warmup_timer(AlertRaiser self)
{
    eas_timer_stop(self->warmup_timer);
    self->is_warmup_timer_running = false;
}

static void stop_cooldown_timer(AlertRaiser self)
{
    eas_timer_stop(self->cooldown_timer);
    self->is_cooldown_timer_running = false;
}

AlertRaiser alert_raiser_create()
{
    EAS_ASSERT(instance_idx < CONFIG_ALERT_RAISER_MAX_NUM_INSTANCES);
    struct AlertRaiserStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->warmup_timer = eas_timer_create(0, warmup_period_expired_cb, instance);
    instance->warmup_period_ms = 0;
    instance->is_warmup_timer_running = false;
    instance->cooldown_timer = eas_timer_create(0, cooldown_period_expired_cb, instance);
    instance->cooldown_period_ms = 0;
    instance->is_cooldown_timer_running = false;
    instance->alert_id = 0;
    instance->is_raised = false;

    return instance;
}

void alert_raiser_set_alert(AlertRaiser self, uint8_t alert_id, uint32_t warmup_period_ms, uint32_t cooldown_period_ms)
{
    if (warmup_period_ms > 0) {
        eas_timer_set_period(self->warmup_timer, warmup_period_ms);
    }
    if (cooldown_period_ms > 0) {
        eas_timer_set_period(self->cooldown_timer, cooldown_period_ms);
    }

    self->warmup_period_ms = warmup_period_ms;
    self->cooldown_period_ms = cooldown_period_ms;
    self->alert_id = alert_id;
}

void alert_raiser_set_alert_condition_result(AlertRaiser self, bool alert_condition_result)
{
    if (self->is_raised == alert_condition_result) {
        /* If the alarm is raised, only the cooldown timer can be running to silence the alarm later. If the alarm is
         * silenced, only the warmup timer can be running to raise the alarm later. */
        bool is_timer_running = self->is_raised ? self->is_cooldown_timer_running : self->is_warmup_timer_running;
        if (is_timer_running) {
            self->is_raised ? stop_cooldown_timer(self) : stop_warmup_timer(self);
        }
        return;
    }

    uint32_t period_ms = alert_condition_result ? self->warmup_period_ms : self->cooldown_period_ms;
    if (period_ms > 0) {
        bool is_timer_running =
            alert_condition_result ? self->is_warmup_timer_running : self->is_cooldown_timer_running;
        if (!is_timer_running) {
            alert_condition_result ? start_warmup_timer(self) : start_cooldown_timer(self);
        }
    } else {
        alert_notifier_notify(self->alert_id, alert_condition_result);
        self->is_raised = alert_condition_result;
    }
}
