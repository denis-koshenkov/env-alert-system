#ifndef VARIABLE_REQUIREMENT_PRIVATE_H
#define VARIABLE_REQUIREMENT_PRIVATE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#include "variable_requirement.h"

typedef struct VariableRequirementInterfaceStruct {
    bool (*evaluate)(VariableRequirement);
} VariableRequirementInterfaceStruct;

typedef struct VariableRequirementStruct {
    VariableRequirementInterfaceStruct *vtable;
    VariableRequirementOperator operator;
} VariableRequirementStruct;

#ifdef __cplusplus
}
#endif

#endif
