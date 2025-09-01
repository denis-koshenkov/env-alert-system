#include <stddef.h>

#include "alert_raiser.h"
#include "alert_notifier.h"
#include "eas_assert.h"

#ifndef CONFIG_ALERT_RAISER_MAX_NUM_INSTANCES
#define CONFIG_ALERT_RAISER_MAX_NUM_INSTANCES 1
#endif

struct AlertRaiserStruct {};

static struct AlertRaiserStruct instances[CONFIG_ALERT_RAISER_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

AlertRaiser alert_raiser_create()
{
    EAS_ASSERT(instance_idx < CONFIG_ALERT_RAISER_MAX_NUM_INSTANCES);
    struct AlertRaiserStruct *instance = &instances[instance_idx];
    instance_idx++;

    return instance;
}

void alert_raiser_set_alert(AlertRaiser self, uint8_t alert_id, uint32_t warmup_period_ms, uint32_t cooldown_period_ms)
{
}

void alert_raiser_set_alert_condition_result(AlertRaiser self, bool alert_condition_result)
{
    alert_notifier_notify(0, true);
}
