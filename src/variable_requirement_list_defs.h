#ifndef VARIABLE_REQUIREMENT_LIST_DEFS_H
#define VARIABLE_REQUIREMENT_LIST_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "variable_requirement_defs.h"

/**
 * @brief Variable requirement list public definitions.
 *
 * The for each callback definition present here so that wrapper modules for one instance of VariableRequirementList,
 * such as TemperatureRequirementList, could use this definition as well in their public API.
 *
 * This is a separate header so that the wrapper modules do not have to include the whole variable_requirement_list.h
 * file in their public .h file.
 */

/**
 * @brief Defines callback type to execute for each element as a part of @ref variable_requirement_list_for_each.
 *
 * @ref variable_requirement_list_for_each executes this callback for each variable requirement present in the list.
 *
 * @param variable_requirement Variable requirement instance for which the callback is being executed.
 */
typedef void (*VariableRequirementListForEachCb)(VariableRequirement variable_requirement);

#ifdef __cplusplus
}
#endif

#endif
