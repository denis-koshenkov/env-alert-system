#ifndef VARIABLE_REQUIREMENTS_VTABLE_NULL_REQUIREMENT_H
#define VARIABLE_REQUIREMENTS_VTABLE_NULL_REQUIREMENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "variable_requirement.h"

VariableRequirement vtable_null_requirement_create();

void vtable_null_requirement_destroy(VariableRequirement vtable_null_requirement);

#ifdef __cplusplus
}
#endif

#endif
