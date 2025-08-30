#include <stddef.h>

#include "alert_condition.h"
#include "eas_assert.h"
#include "config.h"

#ifndef CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES
#define CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES 1
#endif

struct AlertConditionStruct {
    bool requirement_added;
};

static struct AlertConditionStruct instances[CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

AlertCondition alert_condition_create()
{
    EAS_ASSERT(instance_idx < CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES);
    struct AlertConditionStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->requirement_added = false;
    return instance;
}

void alert_condition_add_variable_requirement(AlertCondition self, VariableRequirement variable_requirement)
{
    self->requirement_added = true;
}

bool alert_condition_evaluate(AlertCondition self)
{
    if (self->requirement_added) {
        return true;
    } else {
        EAS_ASSERT(false);
        return false;
    }
}
