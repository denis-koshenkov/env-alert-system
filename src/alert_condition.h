#ifndef ALERT_CONDITION_H
#define ALERT_CONDITION_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "variable_requirement_defs.h"

typedef struct AlertConditionStruct *AlertCondition;

AlertCondition alert_condition_create();

void alert_condition_add_variable_requirement(AlertCondition self, VariableRequirement variable_requirement);

bool alert_condition_evaluate(AlertCondition self);

#ifdef __cplusplus
}
#endif

#endif
