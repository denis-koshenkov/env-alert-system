#include <stdbool.h>

#include "humidity_requirement_list.h"
#include "variable_requirement_list.h"

static VariableRequirementList get_instance()
{
    static VariableRequirementList instance;
    static bool is_created = false;
    if (!is_created) {
        instance = variable_requirement_list_create();
        is_created = true;
    }
    return instance;
}

void humidity_requirement_list_add(VariableRequirement humidity_requirement)
{
    variable_requirement_list_add(get_instance(), humidity_requirement);
}

void humidity_requirement_list_for_each(VariableRequirementListForEachCb cb)
{
    variable_requirement_list_for_each(get_instance(), cb);
}

void humidity_requirement_list_remove_all_for_alert(uint8_t alert_id)
{
    variable_requirement_list_remove_all_for_alert(get_instance(), alert_id);
}
