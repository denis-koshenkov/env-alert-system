#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "alert_condition.h"
#include "fake_variable_requirement.h"

static VariableRequirement variable_requirement;

// clang-format off
TEST_GROUP(AlertCondition)
{
    void setup()
    {
        variable_requirement = fake_variable_requirement_create();
    }

    void teardown()
    {
        variable_requirement_destroy(variable_requirement);
    }
};
// clang-format on

TEST(AlertCondition, EvaluateAssertsNoVariableRequirementsAdded)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("0", "alert_condition_evaluate");

    AlertCondition alert_condition = alert_condition_create();
    bool unused = alert_condition_evaluate(alert_condition);
}

TEST(AlertCondition, EvaluateTrue1ReqTrue)
{
    bool expected_evaluate_result = true;
    fake_variable_requirement_set_evaluate_result(variable_requirement, expected_evaluate_result);

    AlertCondition alert_condition = alert_condition_create();
    alert_condition_add_variable_requirement(alert_condition, variable_requirement);
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(expected_evaluate_result, actual_evaluate_result);
}
