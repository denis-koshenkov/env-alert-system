#ifndef VARIABLE_REQUIREMENT_LIST_H
#define VARIABLE_REQUIREMENT_LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "variable_requirement.h"

/**
 * @brief Variable requirement list.
 */
typedef struct VariableRequirementListStruct *VariableRequirementList;

/**
 * @brief Defines callback type to execute for each element as a part of @ref variable_requirement_list_for_each.
 *
 * @ref variable_requirement_list_for_each executes this callback for each variable requirement present in the list.
 *
 * @param variable_requirement Variable requirement instance for which the callback is being executed.
 */
typedef void (*VariableRequirementListForEachCb)(VariableRequirement variable_requirement);

/**
 * @brief Create a variable requirement list instance.
 *
 * @return VariableRequirementList Created variable requirement list instance.
 */
VariableRequirementList variable_requirement_list_create();

/**
 * @brief Add a variable requirement to the list.
 *
 * @param self Variable requirement list instance returned by @ref variable_requirement_list_create.
 * @param variable_requirement Variable requirement to add to the list.
 */
void variable_requirement_list_add(VariableRequirementList self, VariableRequirement variable_requirement);

/**
 * @brief Execute a callback for each variable requirement in the list.
 *
 * @param self Variable requirement list instance returned by @ref variable_requirement_list_create.
 * @param cb Callback to execute for each variable requirement currently in the list.
 *
 * @note Fires an assert if @p cb is NULL. Calling this function with @p cb equal to NULL would be equivalent to not
 * calling this function at all.
 */
void variable_requirement_list_for_each(VariableRequirementList self, VariableRequirementListForEachCb cb);

/**
 * @brief Remove all variable requirements from the list that belong to a specific alert.
 *
 * @param self Variable requirement list instance returned by @ref variable_requirement_list_create.
 * @param alert_id All variable requirements that belong to this alert are removed from the list.
 */
void variable_requirement_list_remove_all_for_alert(VariableRequirementList self, uint8_t alert_id);

#ifdef __cplusplus
}
#endif

#endif
