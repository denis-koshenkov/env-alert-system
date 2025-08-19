#ifndef VARIABLE_REQUIREMENT_H
#define VARIABLE_REQUIREMENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

typedef enum VariableRequirementOperator {
    /// Greater or equal to.
    VARIABLE_REQUIREMENT_OPERATOR_GEQ,
    /// Less than or equal to.
    VARIABLE_REQUIREMENT_OPERATOR_LEQ,

    /// Invalid operator - do not use.
    VARIABLE_REQUIREMENT_OPERATOR_INVALID,
} VariableRequirementOperator;

typedef struct VariableRequirementStruct *VariableRequirement;

bool variable_requirement_evaluate(VariableRequirement self);

bool variable_requirement_is_result_changed(VariableRequirement self);

uint8_t variable_requirement_get_alert_id(VariableRequirement self);

#ifdef __cplusplus
}
#endif

#endif
