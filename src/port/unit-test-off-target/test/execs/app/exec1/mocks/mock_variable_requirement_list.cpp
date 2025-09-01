#include "CppUTestExt/MockSupport.h"
#include "mock_variable_requirement_list.h"

struct VariableRequirementListStruct {};

VariableRequirementList variable_requirement_list_create()
{
    mock().actualCall("variable_requirement_list_create");
    return (VariableRequirementList)mock().pointerReturnValue();
}

void variable_requirement_list_add(VariableRequirementList self, VariableRequirement variable_requirement)
{
    mock()
        .actualCall("variable_requirement_list_add")
        .withParameter("self", self)
        .withParameter("variable_requirement", variable_requirement);
}

void variable_requirement_list_for_each(VariableRequirementList self, VariableRequirementListForEachCb cb)
{
    mock()
        .actualCall("variable_requirement_list_for_each")
        .withParameter("self", self)
        .withParameterOfType("VariableRequirementListForEachCb", "cb", (const void *)cb);
}

void variable_requirement_list_remove_all_for_alert(VariableRequirementList self, uint8_t alert_id)
{
    mock()
        .actualCall("variable_requirement_list_remove_all_for_alert")
        .withParameter("self", self)
        .withParameter("alert_id", alert_id);
}
