#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "alert_condition.h"
#include "fake_variable_requirement.h"

#define TEST_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS 2

static VariableRequirement variable_requirements[TEST_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS];

// clang-format off
TEST_GROUP(AlertCondition)
{
    void setup()
    {
        for (size_t i = 0; i < TEST_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
            variable_requirements[i] = fake_variable_requirement_create();
        }
    }

    void teardown()
    {
        for (size_t i = 0; i < TEST_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
            variable_requirement_destroy(variable_requirements[i]);
        }
    }
};
// clang-format on

TEST(AlertCondition, EvaluateAssertsNoVariableRequirementsAdded)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("self->variable_requirement_0", "alert_condition_evaluate");

    AlertCondition alert_condition = alert_condition_create();
    bool unused = alert_condition_evaluate(alert_condition);
}

TEST(AlertCondition, EvaluateTrue1ReqTrue)
{
    VariableRequirement variable_requirement = variable_requirements[0];
    bool expected_evaluate_result = true;
    fake_variable_requirement_set_evaluate_result(variable_requirement, expected_evaluate_result);

    AlertCondition alert_condition = alert_condition_create();
    alert_condition_add_variable_requirement(alert_condition, variable_requirement);
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(expected_evaluate_result, actual_evaluate_result);
}

TEST(AlertCondition, EvaluateFalse1ReqFalse)
{
    VariableRequirement variable_requirement = variable_requirements[0];
    bool expected_evaluate_result = false;
    fake_variable_requirement_set_evaluate_result(variable_requirement, expected_evaluate_result);

    AlertCondition alert_condition = alert_condition_create();
    alert_condition_add_variable_requirement(alert_condition, variable_requirement);
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(expected_evaluate_result, actual_evaluate_result);
}

TEST(AlertCondition, EvaluateReturnsFalse2ReqsFalseOrFalse)
{
    fake_variable_requirement_set_evaluate_result(variable_requirements[0], false);
    fake_variable_requirement_set_evaluate_result(variable_requirements[1], false);

    AlertCondition alert_condition = alert_condition_create();
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[0]);
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[1]);
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(false, actual_evaluate_result);
}

TEST(AlertCondition, EvaluateReturnsTrue2ReqsFalseOrTrue)
{
    fake_variable_requirement_set_evaluate_result(variable_requirements[0], false);
    fake_variable_requirement_set_evaluate_result(variable_requirements[1], true);

    AlertCondition alert_condition = alert_condition_create();
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[0]);
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[1]);
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(true, actual_evaluate_result);
}

TEST(AlertCondition, EvaluateReturnsTrue2ReqsTrueOrFalse)
{
    fake_variable_requirement_set_evaluate_result(variable_requirements[0], true);
    fake_variable_requirement_set_evaluate_result(variable_requirements[1], false);

    AlertCondition alert_condition = alert_condition_create();
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[0]);
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[1]);
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(true, actual_evaluate_result);
}

TEST(AlertCondition, EvaluateReturnsTrue2ReqsTrueOrTrue)
{
    fake_variable_requirement_set_evaluate_result(variable_requirements[0], true);
    fake_variable_requirement_set_evaluate_result(variable_requirements[1], true);

    AlertCondition alert_condition = alert_condition_create();
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[0]);
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[1]);
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(true, actual_evaluate_result);
}
