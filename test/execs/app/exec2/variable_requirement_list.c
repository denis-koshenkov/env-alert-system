#include "CppUTest/TestHarness_c.h"

#include "variable_requirement_list.h"
#include "pressure_requirement.h"

static VariableRequirement pressure_requirement;

static VariableRequirement var_req_in_list = NULL;
static uint8_t count = 0;
static void for_each_cb(VariableRequirement variable_requirement)
{
    var_req_in_list = variable_requirement;
    count++;
}

TEST_GROUP_C_SETUP(VariableRequirementList)
{
    pressure_requirement = pressure_requirement_create(0, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 0);

    var_req_in_list = NULL;
    count = 0;
}

TEST_GROUP_C_TEARDOWN(VariableRequirementList)
{
    variable_requirement_destroy(pressure_requirement);
}

TEST_C(VariableRequirementList, ListIsEmptyAfterCreate)
{
    VariableRequirementList list = variable_requirement_list_create();
    variable_requirement_list_for_each(list, for_each_cb);
    CHECK_C(var_req_in_list == NULL);
    CHECK_EQUAL_C_UINT(0, count);
}

TEST_C(VariableRequirementList, ListContainsOneVarWhenOneVarAdded)
{
    VariableRequirementList list = variable_requirement_list_create();
    variable_requirement_list_add(list, pressure_requirement);
    variable_requirement_list_for_each(list, for_each_cb);

    CHECK_EQUAL_C_POINTER(pressure_requirement, var_req_in_list);
    CHECK_EQUAL_C_UINT(1, count);
}
