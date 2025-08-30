#include <stddef.h>

#include "alert_condition.h"
#include "eas_assert.h"
#include "config.h"
#include "variable_requirement_evaluator.h"

#ifndef CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES
#define CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES 1
#endif

struct AlertConditionStruct {
    VariableRequirement variable_requirement;
};

static struct AlertConditionStruct instances[CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

AlertCondition alert_condition_create()
{
    EAS_ASSERT(instance_idx < CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES);
    struct AlertConditionStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->variable_requirement = NULL;
    return instance;
}

void alert_condition_add_variable_requirement(AlertCondition self, VariableRequirement variable_requirement)
{
    self->variable_requirement = variable_requirement;
}

bool alert_condition_evaluate(AlertCondition self)
{
    EAS_ASSERT(self->variable_requirement);
    return variable_requirement_evaluator_evaluate(self->variable_requirement);
}
