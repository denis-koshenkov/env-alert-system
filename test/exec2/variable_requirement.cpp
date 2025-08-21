#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "variable_requirement.h"
#include "mocks/variable_requirements/mock_variable_requirement.h"

/* ------------------------------------------ VariableRequirement test group ------------------------------------ */

TEST_GROUP(VariableRequirement){};

TEST(VariableRequirement, evaluateRaisesAssertIfCalledWithNullPointer)
{
    TestAssertPlugin::expectAssertion("self");
    bool unused = variable_requirement_evaluate(NULL);
}

TEST(VariableRequirement, isResultChangedRaisesAssertIfCalledWithNullPointer)
{
    TestAssertPlugin::expectAssertion("self");
    bool unused = variable_requirement_is_result_changed(NULL);
}

TEST(VariableRequirement, getAlertIdRaisesAssertIfCalledWithNullPointer)
{
    TestAssertPlugin::expectAssertion("self");
    uint8_t unused = variable_requirement_get_alert_id(NULL);
}

TEST(VariableRequirement, destroyRaisesAssertIfCalledWithNullPointer)
{
    TestAssertPlugin::expectAssertion("self");
    variable_requirement_destroy(NULL);
}

/* ------------------------------------------ VariableRequirementMock test group -------------------------- */

static VariableRequirement mock_variable_requirement;

// clang-format off
TEST_GROUP(VariableRequirementMock)
{
    void setup()
    {
        mock_variable_requirement = mock_variable_requirement_create(
            MOCK_VARIABLE_REQUIREMENT_PASS_VALID_INSTANCE_TO_VAR_REQ_CREATE,
            MOCK_VARIABLE_REQUIREMENT_PASS_GEQ_OPERATOR_TO_VAR_REQ_CREATE,
            MOCK_VARIABLE_REQUIREMENT_PASS_VALID_VTABLE_TO_VAR_REQ_CREATE
        );
    }

    void teardown()
    {
        variable_requirement_destroy(mock_variable_requirement);
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

TEST(VariableRequirementMock, isResultChangedReturnsFalseAfterTwoCallsToEvaluateReturnFalse)
{
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, false);
    bool evaluate_result_1 = variable_requirement_evaluate(mock_variable_requirement);
    bool evaluate_result_2 = variable_requirement_evaluate(mock_variable_requirement);
    bool is_result_changed = variable_requirement_is_result_changed(mock_variable_requirement);

    CHECK(!evaluate_result_1);
    CHECK(!evaluate_result_2);
    CHECK(!is_result_changed);
}

TEST(VariableRequirementMock, isResultChangedReturnsTrueFirstEvaluateTrueSecondEvaluateFalse)
{
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, true);
    bool evaluate_result_1 = variable_requirement_evaluate(mock_variable_requirement);
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, false);
    bool evaluate_result_2 = variable_requirement_evaluate(mock_variable_requirement);
    bool is_result_changed = variable_requirement_is_result_changed(mock_variable_requirement);

    CHECK(evaluate_result_1);
    CHECK(!evaluate_result_2);
    CHECK(is_result_changed);
}

TEST(VariableRequirementMock, isResultChangedReturnsTrueFirstEvaluateFalseSecondEvaluateTrue)
{
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, false);
    bool evaluate_result_1 = variable_requirement_evaluate(mock_variable_requirement);
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, true);
    bool evaluate_result_2 = variable_requirement_evaluate(mock_variable_requirement);
    bool is_result_changed = variable_requirement_is_result_changed(mock_variable_requirement);

    CHECK(!evaluate_result_1);
    CHECK(evaluate_result_2);
    CHECK(is_result_changed);
}

TEST(VariableRequirementMock, isResultChangedReturnsTrueManyEvaluationsLastTwoDifferent)
{
    bool dummy;
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, false);
    dummy = variable_requirement_evaluate(mock_variable_requirement);
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, true);
    dummy = variable_requirement_evaluate(mock_variable_requirement);
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, true);
    dummy = variable_requirement_evaluate(mock_variable_requirement);
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, true);
    dummy = variable_requirement_evaluate(mock_variable_requirement);
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, true);
    bool evaluate_result_1 = variable_requirement_evaluate(mock_variable_requirement);
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, false);
    bool evaluate_result_2 = variable_requirement_evaluate(mock_variable_requirement);

    bool is_result_changed = variable_requirement_is_result_changed(mock_variable_requirement);

    CHECK(evaluate_result_1);
    CHECK(!evaluate_result_2);
    CHECK(is_result_changed);
}

TEST(VariableRequirementMock, isResultChangedReturnsFalseManyEvaluationsLastTwoSame)
{
    bool dummy;
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, true);
    dummy = variable_requirement_evaluate(mock_variable_requirement);
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, false);
    dummy = variable_requirement_evaluate(mock_variable_requirement);
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, true);
    dummy = variable_requirement_evaluate(mock_variable_requirement);
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, false);
    dummy = variable_requirement_evaluate(mock_variable_requirement);
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, true);
    bool evaluate_result_1 = variable_requirement_evaluate(mock_variable_requirement);
    mock_variable_requirement_set_evaluate_result(mock_variable_requirement, true);
    bool evaluate_result_2 = variable_requirement_evaluate(mock_variable_requirement);

    bool is_result_changed = variable_requirement_is_result_changed(mock_variable_requirement);

    CHECK(evaluate_result_1);
    CHECK(evaluate_result_2);
    CHECK(!is_result_changed);
}

/* ------------------------------------------ VariableRequirementMockCreate test group -------------------------- */

/* This is its own test group, because we expect asserts during the calls to mock_variable_requirement_create(). But in
 the other test group that uses mock_variable_requirement, mock_variable_requirement_create() is called in setup(), so
 we cannot expect asserts there. In this test group, calls to mock_variable_requirement_create() are performed in the
 test body, which allows us to expect asserts there. */

static VariableRequirement mock_variable_requirement_create_test_group;

// clang-format off
TEST_GROUP(VariableRequirementMockCreate)
{
    void teardown()
    {
        /* This cannot be variable_requirement_destroy(), because that will access the vtable. The tests in this test
        group will not get past the asserts at the beginning of variable_requirement_create, so vtable will never be
        populated. This means that at the end of the test, when we end up here in the teardown, the vtable field contains
        an invalid pointer. But we still need to free the memory we allocated in mock_variable_requirement_create(), so
        we call the destroy() here directly. */
        mock_variable_requirement_destroy(mock_variable_requirement_create_test_group);
    }
};
// clang-format on

TEST(VariableRequirementMockCreate, createRaisesAssertIfSelfIsNull)
{
    TestAssertPlugin::expectAssertion("self");
    mock_variable_requirement_create_test_group =
        mock_variable_requirement_create(MOCK_VARIABLE_REQUIREMENT_PASS_NULLPTR_INSTANCE_TO_VAR_REQ_CREATE,
                                         MOCK_VARIABLE_REQUIREMENT_PASS_GEQ_OPERATOR_TO_VAR_REQ_CREATE,
                                         MOCK_VARIABLE_REQUIREMENT_PASS_VALID_VTABLE_TO_VAR_REQ_CREATE);
}

TEST(VariableRequirementMockCreate, createRaisesAssertIfOperatorIsInvalid)
{
    TestAssertPlugin::expectAssertion("is_valid_operator(operator)");
    mock_variable_requirement_create_test_group =
        mock_variable_requirement_create(MOCK_VARIABLE_REQUIREMENT_PASS_VALID_INSTANCE_TO_VAR_REQ_CREATE,
                                         MOCK_VARIABLE_REQUIREMENT_PASS_INVALID_OPERATOR_TO_VAR_REQ_CREATE,
                                         MOCK_VARIABLE_REQUIREMENT_PASS_VALID_VTABLE_TO_VAR_REQ_CREATE);
}

TEST(VariableRequirementMockCreate, createRaisesAssertIfVtableIsNull)
{
    TestAssertPlugin::expectAssertion("vtable");
    mock_variable_requirement_create_test_group =
        mock_variable_requirement_create(MOCK_VARIABLE_REQUIREMENT_PASS_VALID_INSTANCE_TO_VAR_REQ_CREATE,
                                         MOCK_VARIABLE_REQUIREMENT_PASS_GEQ_OPERATOR_TO_VAR_REQ_CREATE,
                                         MOCK_VARIABLE_REQUIREMENT_PASS_NULL_VTABLE_TO_VAR_REQ_CREATE);
}

TEST(VariableRequirementMockCreate, createRaisesAssertIfEvaluateIsNull)
{
    TestAssertPlugin::expectAssertion("vtable->evaluate");
    mock_variable_requirement_create_test_group =
        mock_variable_requirement_create(MOCK_VARIABLE_REQUIREMENT_PASS_VALID_INSTANCE_TO_VAR_REQ_CREATE,
                                         MOCK_VARIABLE_REQUIREMENT_PASS_GEQ_OPERATOR_TO_VAR_REQ_CREATE,
                                         MOCK_VARIABLE_REQUIREMENT_PASS_NULL_EVALUATE_TO_VAR_REQ_CREATE);
}

TEST(VariableRequirementMockCreate, createRaisesAssertIfDestroyIsNull)
{
    TestAssertPlugin::expectAssertion("vtable->destroy");
    mock_variable_requirement_create_test_group =
        mock_variable_requirement_create(MOCK_VARIABLE_REQUIREMENT_PASS_VALID_INSTANCE_TO_VAR_REQ_CREATE,
                                         MOCK_VARIABLE_REQUIREMENT_PASS_GEQ_OPERATOR_TO_VAR_REQ_CREATE,
                                         MOCK_VARIABLE_REQUIREMENT_PASS_NULL_DESTROY_TO_VAR_REQ_CREATE);
}
