#include <stddef.h>

#include "alert_condition.h"
#include "eas_assert.h"

#ifndef CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES
#define CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES 1
#endif

struct AlertConditionStruct {};

static struct AlertConditionStruct instances[CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

AlertCondition alert_condition_create()
{
    EAS_ASSERT(instance_idx < CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES);
    struct AlertConditionStruct *instance = &instances[instance_idx];
    instance_idx++;

    return instance;
}

bool alert_condition_evaluate(AlertCondition self)
{
    EAS_ASSERT(false);
    return false;
}
