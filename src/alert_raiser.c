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
};

static struct AlertRaiserStruct instances[CONFIG_ALERT_RAISER_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

static void warmup_period_expired_cb(void *user_data)
{
    alert_notifier_notify(0, true);
}

static void cooldown_period_expired_cb(void *user_data)
{
}

AlertRaiser alert_raiser_create()
{
    EAS_ASSERT(instance_idx < CONFIG_ALERT_RAISER_MAX_NUM_INSTANCES);
    struct AlertRaiserStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->warmup_timer = eas_timer_create(0, warmup_period_expired_cb, NULL);
    eas_timer_create(0, cooldown_period_expired_cb, NULL);
    instance->warmup_period_ms = 0;

    return instance;
}

void alert_raiser_set_alert(AlertRaiser self, uint8_t alert_id, uint32_t warmup_period_ms, uint32_t cooldown_period_ms)
{
    if (warmup_period_ms > 0) {
        eas_timer_set_period(self->warmup_timer, warmup_period_ms);
    }
    self->warmup_period_ms = warmup_period_ms;
}

void alert_raiser_set_alert_condition_result(AlertRaiser self, bool alert_condition_result)
{
    if (self->warmup_period_ms > 0 && alert_condition_result) {
        eas_timer_start(self->warmup_timer);
    } else {
        alert_notifier_notify(0, alert_condition_result);
    }
}
