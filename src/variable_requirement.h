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
} VariableRequirementOperator;

typedef struct VariableRequirementStruct *VariableRequirement;

bool variable_requirement_evaluate(VariableRequirement self);

bool variable_requirement_is_result_changed(VariableRequirement self);

#ifdef __cplusplus
}
#endif

#endif
