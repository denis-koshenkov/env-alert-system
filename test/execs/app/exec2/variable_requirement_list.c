#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport_c.h"

#include "variable_requirement_list.h"
/* We test with adding actual variable requirements to the list, so this header is needed because we need to create
 * pressure requirements to add to the list. */
#include "pressure_requirement.h"
#include "fake_variable_requirement_allocator.h"

static VariableRequirement pressure_requirement;
static void *requirement_buffer;

static VariableRequirement var_req_in_list = NULL;
static uint8_t count = 0;
static void for_each_cb(VariableRequirement variable_requirement)
{
    var_req_in_list = variable_requirement;
    count++;
}

TEST_GROUP_C_SETUP(VariableRequirementList)
{
    requirement_buffer = fake_variable_requirement_allocator_alloc();
    mock_c()->expectOneCall("variable_requirement_allocator_alloc")->andReturnPointerValue(requirement_buffer);
    pressure_requirement = pressure_requirement_create(0, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 0);

    var_req_in_list = NULL;
    count = 0;
}

TEST_GROUP_C_TEARDOWN(VariableRequirementList)
{
    mock_c()->expectOneCall("variable_requirement_allocator_free")->withPointerParameters("buf", requirement_buffer);
    variable_requirement_destroy(pressure_requirement);
    fake_variable_requirement_allocator_free(requirement_buffer);
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
