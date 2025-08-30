#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport_c.h"

#include "variable_requirement_list.h"
/* We test with adding actual variable requirements to the list, so this header is needed because we need to create
 * pressure requirements to add to the list. */
#include "pressure_requirement.h"
#include "fake_variable_requirement_allocator.h"
#include "fake_linked_list_node_allocator.h"
#include "eas_assert.h"

#define TEST_VARIABLE_REQUIREMENT_LIST_MAX_NUM_EXPECTED_REQUIREMENTS 3

typedef struct ExpectedVariableRequirement {
    void *linked_list_node_buffer;
    void *requirement_buffer;
    VariableRequirement requirement;
    bool is_expected;
} ExpectedVariableRequirement;

static ExpectedVariableRequirement expected_requirements[TEST_VARIABLE_REQUIREMENT_LIST_MAX_NUM_EXPECTED_REQUIREMENTS];

static void expect_requirement_in_list(size_t index)
{
    EAS_ASSERT(index < TEST_VARIABLE_REQUIREMENT_LIST_MAX_NUM_EXPECTED_REQUIREMENTS);
    expected_requirements[index].is_expected = true;
}

static void actual_requirement_detected_in_list(VariableRequirement requirement)
{
    for (size_t i = 0; i < TEST_VARIABLE_REQUIREMENT_LIST_MAX_NUM_EXPECTED_REQUIREMENTS; i++) {
        if (expected_requirements[i].requirement == requirement) {
            /* Initially all is_expected flags are false. During the test setup step, is_expected flag is set for
             * all requirements that are expected to be in the list. During the test exercise step, we flip the flags
             * for all variables that were actually detected in the list. If all and only the required variables were in
             * the list, all flags will be false by the end of the test. Otherwise, some flags will be true - either
             * variables that were expected in the list but were not in the list, or variables that were in the list
             * but were not expected to be there. */
            expected_requirements[i].is_expected = !expected_requirements[i].is_expected;
            return;
        }
    }

    /* Invalid requirement - it is not in the expected requirements list */
    EAS_ASSERT(false);
}

static bool expected_requirements_match_actual()
{
    for (size_t i = 0; i < TEST_VARIABLE_REQUIREMENT_LIST_MAX_NUM_EXPECTED_REQUIREMENTS; i++) {
        if (expected_requirements[i].is_expected) {
            /* This variable either was expected to be in the list but was not there, or it was not
             * expected to be in the list but was there. */
            return false;
        }
    }
    return true;
}

static void for_each_cb_expected_requirements(VariableRequirement variable_requirement)
{
    actual_requirement_detected_in_list(variable_requirement);
}

TEST_GROUP_C_SETUP(VariableRequirementList)
{
    for (size_t i = 0; i < TEST_VARIABLE_REQUIREMENT_LIST_MAX_NUM_EXPECTED_REQUIREMENTS; i++) {
        expected_requirements[i].linked_list_node_buffer = fake_linked_list_node_allocator_alloc();
        expected_requirements[i].requirement_buffer = fake_variable_requirement_allocator_alloc();
        mock_c()
            ->expectOneCall("variable_requirement_allocator_alloc")
            ->andReturnPointerValue(expected_requirements[i].requirement_buffer);
        expected_requirements[i].requirement = pressure_requirement_create(0, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 0);
        expected_requirements[i].is_expected = false;
    }
}

TEST_GROUP_C_TEARDOWN(VariableRequirementList)
{
    for (size_t i = 0; i < TEST_VARIABLE_REQUIREMENT_LIST_MAX_NUM_EXPECTED_REQUIREMENTS; i++) {
        mock_c()
            ->expectOneCall("variable_requirement_allocator_free")
            ->withPointerParameters("buf", expected_requirements[i].requirement_buffer);
        variable_requirement_destroy(expected_requirements[i].requirement);
        fake_variable_requirement_allocator_free(expected_requirements[i].requirement_buffer);
        fake_linked_list_node_allocator_free(expected_requirements[i].linked_list_node_buffer);
    }
}

TEST_C(VariableRequirementList, ListIsEmptyAfterCreate)
{
    VariableRequirementList list = variable_requirement_list_create();
    variable_requirement_list_for_each(list, for_each_cb_expected_requirements);

    CHECK_C(expected_requirements_match_actual());
}

TEST_C(VariableRequirementList, ListContainsOneVarWhenOneVarAdded)
{
    mock_c()
        ->expectOneCall("linked_list_node_allocator_alloc")
        ->andReturnPointerValue(expected_requirements[0].linked_list_node_buffer);
    expect_requirement_in_list(0);

    VariableRequirementList list = variable_requirement_list_create();
    variable_requirement_list_add(list, expected_requirements[0].requirement);
    variable_requirement_list_for_each(list, for_each_cb_expected_requirements);

    CHECK_C(expected_requirements_match_actual());
}

TEST_C(VariableRequirementList, ListContainsThreeVarsWhenThreeVarsAdded)
{
    mock_c()
        ->expectOneCall("linked_list_node_allocator_alloc")
        ->andReturnPointerValue(expected_requirements[0].linked_list_node_buffer);
    mock_c()
        ->expectOneCall("linked_list_node_allocator_alloc")
        ->andReturnPointerValue(expected_requirements[1].linked_list_node_buffer);
    mock_c()
        ->expectOneCall("linked_list_node_allocator_alloc")
        ->andReturnPointerValue(expected_requirements[2].linked_list_node_buffer);
    expect_requirement_in_list(0);
    expect_requirement_in_list(1);
    expect_requirement_in_list(2);

    VariableRequirementList list = variable_requirement_list_create();
    variable_requirement_list_add(list, expected_requirements[0].requirement);
    variable_requirement_list_add(list, expected_requirements[1].requirement);
    variable_requirement_list_add(list, expected_requirements[2].requirement);

    variable_requirement_list_for_each(list, for_each_cb_expected_requirements);

    CHECK_C(expected_requirements_match_actual());
}
