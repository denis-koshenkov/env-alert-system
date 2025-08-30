#ifndef TEMPERATURE_REQUIREMENT_LIST_H
#define TEMPERATURE_REQUIREMENT_LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "variable_requirement_list_defs.h"

void temperature_requirement_list_add(VariableRequirement temperature_requirement);

void temperature_requirement_list_for_each(VariableRequirementListForEachCb cb);

void temperature_requirement_list_remove_all_for_alert(uint8_t alert_id);

#ifdef __cplusplus
}
#endif

#endif
