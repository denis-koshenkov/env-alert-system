#ifndef VARIABLE_REQUIREMENT_DEFS_H
#define VARIABLE_REQUIREMENT_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Variable requirement public definitions.
 *
 * The definition of the VariableRequirement type is here, so that other modules can use it without including the whole
 * variable_requirement.h header.
 *
 * For example, variable_requirement_list.h only needs to know about the VariableRequirement type definition, it should
 * not expose the whole public API defined in variable_requirement.h.
 */

typedef struct VariableRequirementStruct *VariableRequirement;

#ifdef __cplusplus
}
#endif

#endif
