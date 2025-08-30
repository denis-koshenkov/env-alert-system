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

/**
 * @brief Callback to be passed to linked_list_for_each.
 *
 * Executes a callback for each variable requirement.
 *
 * @param element linked_list_remove_on_condition passes the current list element to this parameter. In our case, it is
 * the VariableRequirement instance.
 * @param user_data Function pointer to a callback to execute for each variable requirement.
 */
static void linked_list_for_each_cb(void *element, void *user_data)
{
    VariableRequirementListForEachCb cb = (VariableRequirementListForEachCb)user_data;
    VariableRequirement variable_requirement = (VariableRequirement)element;
    EAS_ASSERT(cb);

    cb(variable_requirement);
}

/**
 * @brief Callback to be passed to linked_list_remove_on_condition.
 *
 * @param element linked_list_remove_on_condition passes the current list element to this parameter. In our case, it is
 * the VariableRequirement instance.
 * @param user_data Pointer to alert id - for all variable requirements with this alert id, this function needs to
 * return true.
 *
 * @return true @p element has alert id equal to alert id passed as @p user_data.
 * @return false @p element has alert id that is NOT equal to alert id passed as @p user_data.
 */
static bool requirement_has_alert_id(void *element, void *user_data)
{
    VariableRequirement variable_requirement = (VariableRequirement)element;
    uint8_t *alert_id = (uint8_t *)user_data;
    EAS_ASSERT(alert_id);

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
