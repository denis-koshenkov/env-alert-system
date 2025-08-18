#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "variable_requirement.h"
#include "mocks/variable_requirements/vtable_null_requirement.h"
#include "mocks/variable_requirements/evaluate_null_requirement.h"
#include "mocks/variable_requirements/mock_variable_requirement.h"

/* ------------------------------------------ VariableRequirement test group ------------------------------------ */

TEST_GROUP(VariableRequirement){};

TEST(VariableRequirement, evaluateRaisesAssertIfCalledWithNullPointer)
{
    TestAssertPlugin::expectAssertion("self");
    variable_requirement_evaluate(NULL);
}

/* ------------------------------------------ VariableRequirementVtableNull test group -------------------------- */

/* VariableRequirementVtableNull is a different test group, because we need to call vtable_null_requirement_destroy() in
 * teardown. If we put it in the test body, it will not be executed, because assert fires and the test ends prematurely.
 * But we should not call vtable_null_requirement_destroy() for the VariableRequirement test group tests. */
static VariableRequirement vtable_null_requirement;

// clang-format off
TEST_GROUP(VariableRequirementVtableNull)
{
    void setup()
    {
        vtable_null_requirement = vtable_null_requirement_create();
    }

    void teardown()
    {
        vtable_null_requirement_destroy(vtable_null_requirement);
    }
};
// clang-format on

TEST(VariableRequirementVtableNull, evaluateRaisesAssertIfVtableIsNull)
{
    TestAssertPlugin::expectAssertion("self->vtable");
    variable_requirement_evaluate(vtable_null_requirement);
}

/* ------------------------------------------ VariableRequirementEvaluateNull test group -------------------------- */

/* Same logic applies to VariableRequirementEvaluateNull test group as to the VariableRequirementVtableNull test
 * group. */
static VariableRequirement evaluate_null_requirement;

// clang-format off
TEST_GROUP(VariableRequirementEvaluateNull)
{
    void setup()
    {
        evaluate_null_requirement = evaluate_null_requirement_create();
    }

    void teardown()
    {
        evaluate_null_requirement_destroy(evaluate_null_requirement);
    }
};
// clang-format on

TEST(VariableRequirementEvaluateNull, evaluateRaisesAssertIfEvaluateIsNull)
{
    TestAssertPlugin::expectAssertion("self->vtable->evaluate");
    variable_requirement_evaluate(evaluate_null_requirement);
}

/* ------------------------------------------ VariableRequirementMock test group -------------------------- */

static VariableRequirement mock_variable_requirement;

// clang-format off
TEST_GROUP(VariableRequirementMock)
{
    void setup()
    {
        mock_variable_requirement = mock_variable_requirement_create();
    }

    void teardown()
    {
        mock_variable_requirement_destroy(mock_variable_requirement);
    }
};
// clang-format on

TEST(VariableRequirementMock, isResultChangedRaisesAssertIfCalledBeforeEvaluate)
{
    TestAssertPlugin::expectAssertion("self->evaluate_has_been_called");
    bool is_result_changed = variable_requirement_is_result_changed(mock_variable_requirement);
}

TEST(VariableRequirementMock, isResultChangedReturnsTrueAfterEvaluateIsCalledOnceReturnedTrue)
{
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, true);
    bool evaluate_result = variable_requirement_evaluate(mock_variable_requirement);
    bool is_result_changed = variable_requirement_is_result_changed(mock_variable_requirement);

    /* Make sure variable_requirement_evaluate() actually returned true. */
    CHECK(evaluate_result);
    CHECK(is_result_changed);
}

TEST(VariableRequirementMock, isResultChangedReturnsTrueAfterEvaluateIsCalledOnceReturnedFalse)
{
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, false);
    bool evaluate_result = variable_requirement_evaluate(mock_variable_requirement);
    bool is_result_changed = variable_requirement_is_result_changed(mock_variable_requirement);

    /* Make sure variable_requirement_evaluate() actually returned false. */
    CHECK(!evaluate_result);
    CHECK(is_result_changed);
}

TEST(VariableRequirementMock, isResultChangedReturnsFalseAfterTwoCallsToEvaluateReturnTrue)
{
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, true);
    bool evaluate_result_1 = variable_requirement_evaluate(mock_variable_requirement);
    bool evaluate_result_2 = variable_requirement_evaluate(mock_variable_requirement);
    bool is_result_changed = variable_requirement_is_result_changed(mock_variable_requirement);

    CHECK(evaluate_result_1);
    CHECK(evaluate_result_2);
    CHECK(!is_result_changed);
}
