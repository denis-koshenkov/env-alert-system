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
    bool evaluate_has_been_called;
} VariableRequirementStruct;

/**
 * @brief Create generic variable requirement.
 *
 * This function should be called as early as possible in the create() functions of the subclasses of
 * VariableRequirement, such as TemperatureVariableRequirement. The implementation of this function initializes all the
 * private members of VariableRequirementStruct that belong to the VariableRequirement class.
 *
 * @param self Variable requirement instance. This should already point to valid memory, memory allocation for variable
 * requirement instances is performed in the create() functions of the subclasses.
 */
void variable_requirement_create(VariableRequirement self);

#ifdef __cplusplus
}
#endif

#endif
