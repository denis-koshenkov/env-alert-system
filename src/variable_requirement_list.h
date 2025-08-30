#ifndef VARIABLE_REQUIREMENT_LIST_H
#define VARIABLE_REQUIREMENT_LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "variable_requirement.h"

typedef struct VariableRequirementListStruct *VariableRequirementList;

typedef void (*VariableRequirementListForEachCb)(VariableRequirement variable_requirement);

VariableRequirementList variable_requirement_list_create();

void variable_requirement_list_add(VariableRequirementList self, VariableRequirement variable_requirement);

void variable_requirement_list_for_each(VariableRequirementList self, VariableRequirementListForEachCb cb);

void variable_requirement_list_remove_all_for_alert(VariableRequirementList self, uint8_t alert_id);

#ifdef __cplusplus
}
#endif

#endif
