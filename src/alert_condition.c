#include <stddef.h>

#include "alert_condition.h"
#include "eas_assert.h"
#include "config.h"
#include "variable_requirement_evaluator.h"

#ifndef CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES
#define CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES 1
#endif

struct AlertConditionStruct {
    VariableRequirement variable_requirement_0;
    VariableRequirement variable_requirement_1;
};

static struct AlertConditionStruct instances[CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

AlertCondition alert_condition_create()
{
    EAS_ASSERT(instance_idx < CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES);
    struct AlertConditionStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->variable_requirement_0 = NULL;
    instance->variable_requirement_1 = NULL;
    return instance;
}

void alert_condition_add_variable_requirement(AlertCondition self, VariableRequirement variable_requirement)
{
    if (self->variable_requirement_0 == NULL) {
        self->variable_requirement_0 = variable_requirement;
    } else {
        self->variable_requirement_1 = variable_requirement;
    }
}

bool alert_condition_evaluate(AlertCondition self)
{
    EAS_ASSERT(self->variable_requirement_0);

    if (self->variable_requirement_1 == NULL) {
        /* Only one requirement */
        return variable_requirement_evaluator_evaluate(self->variable_requirement_0);
    } else {
        /* Two requirements */
        bool result_0 = variable_requirement_evaluator_evaluate(self->variable_requirement_0);
        bool result_1 = variable_requirement_evaluator_evaluate(self->variable_requirement_1);
        return (result_0 || result_1);
    }
}
