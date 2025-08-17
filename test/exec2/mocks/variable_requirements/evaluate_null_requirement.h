#ifndef VARIABLE_REQUIREMENTS_EVALUATE_NULL_REQUIREMENT_H
#define VARIABLE_REQUIREMENTS_EVALUATE_NULL_REQUIREMENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "variable_requirement.h"

VariableRequirement evaluate_null_requirement_create();

void evaluate_null_requirement_destroy(VariableRequirement evaluate_null_requirement);

#ifdef __cplusplus
}
#endif

#endif
