#include <stddef.h>

#include "variable_requirement_list.h"
#include "utils/linked_list.h"
#include "config.h"
#include "eas_assert.h"

#ifndef CONFIG_VARIABLE_REQUIREMENT_LIST_MAX_NUM_INSTANCES
#define CONFIG_VARIABLE_REQUIREMENT_LIST_MAX_NUM_INSTANCES 1
#endif

typedef struct VariableRequirementListStruct {
    LinkedList linked_list;
} VariableRequirementListStruct;

static struct VariableRequirementListStruct instances[CONFIG_VARIABLE_REQUIREMENT_LIST_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

static void linked_list_for_each_cb(void *element, void *user_data)
{
    VariableRequirementListForEachCb cb = (VariableRequirementListForEachCb)user_data;
    VariableRequirement variable_requirement = (VariableRequirement)element;
    cb(variable_requirement);
}

static bool requirement_has_alert_id(void *element, void *user_data)
{
    VariableRequirement variable_requirement = (VariableRequirement)element;
    uint8_t *alert_id = (uint8_t *)user_data;

    return (variable_requirement_get_alert_id(variable_requirement) == (*alert_id));
}

VariableRequirementList variable_requirement_list_create()
{
    EAS_ASSERT(instance_idx < CONFIG_VARIABLE_REQUIREMENT_LIST_MAX_NUM_INSTANCES);
    struct VariableRequirementListStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->linked_list = linked_list_create();
    return instance;
}

void variable_requirement_list_add(VariableRequirementList self, VariableRequirement variable_requirement)
{
    EAS_ASSERT(self);
    linked_list_add(self->linked_list, variable_requirement);
}

void variable_requirement_list_for_each(VariableRequirementList self, VariableRequirementListForEachCb cb)
{
    EAS_ASSERT(self);
    EAS_ASSERT(cb);
    linked_list_for_each(self->linked_list, linked_list_for_each_cb, cb);
}

void variable_requirement_list_remove_all_for_alert(VariableRequirementList self, uint8_t alert_id)
{
    EAS_ASSERT(self);
    linked_list_remove_on_condition(self->linked_list, requirement_has_alert_id, &alert_id);
}
