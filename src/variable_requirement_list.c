#include <stddef.h>

#include "variable_requirement_list.h"
#include "config.h"
#include "eas_assert.h"

#ifndef CONFIG_VARIABLE_REQUIREMENT_LIST_MAX_NUM_INSTANCES
#define CONFIG_VARIABLE_REQUIREMENT_LIST_MAX_NUM_INSTANCES 1
#endif

typedef struct VariableRequirementListStruct {
    VariableRequirement variable_requirement;
} VariableRequirementListStruct;

static struct VariableRequirementListStruct instances[CONFIG_VARIABLE_REQUIREMENT_LIST_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

VariableRequirementList variable_requirement_list_create()
{
    EAS_ASSERT(instance_idx < CONFIG_VARIABLE_REQUIREMENT_LIST_MAX_NUM_INSTANCES);
    struct VariableRequirementListStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->variable_requirement = NULL;
    return instance;
}

void variable_requirement_list_add(VariableRequirementList self, VariableRequirement variable_requirement)
{
    self->variable_requirement = variable_requirement;
}

void variable_requirement_list_for_each(VariableRequirementList self, VariableRequirementListForEachCb cb)
{
    if (self->variable_requirement != NULL) {
        cb(self->variable_requirement);
    }
}
