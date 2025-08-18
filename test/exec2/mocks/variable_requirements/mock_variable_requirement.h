#ifndef VARIABLE_REQUIREMENTS_MOCK_VARIABLE_REQUIREMENT_H
#define VARIABLE_REQUIREMENTS_MOCK_VARIABLE_REQUIREMENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "variable_requirement.h"

VariableRequirement mock_variable_requirement_create();

void mock_variable_requirement_destroy(VariableRequirement mock_variable_requirement);

void mock_variable_requirement_set_evaluate_result(VariableRequirement mock_variable_requirement, bool result);

#ifdef __cplusplus
}
#endif

#endif
