#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "alert_condition.h"
#include "fake_variable_requirement.h"
#include "eas_assert.h"
#include "config.h"

#define TEST_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS 10

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
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("self->num_items_in_reqs_array > 0", "alert_condition_evaluate");

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

TEST(AlertCondition, EvaluateReturnsFalse2ReqsFalseAndFalse)
{
    fake_variable_requirement_set_evaluate_result(variable_requirements[0], false);
    fake_variable_requirement_set_evaluate_result(variable_requirements[1], false);

    AlertCondition alert_condition = alert_condition_create();
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[0]);
    alert_condition_start_new_ored_requirement(alert_condition);
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[1]);
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(false, actual_evaluate_result);
}

TEST(AlertCondition, EvaluateReturnsFalse2ReqsFalseAndTrue)
{
    fake_variable_requirement_set_evaluate_result(variable_requirements[0], false);
    fake_variable_requirement_set_evaluate_result(variable_requirements[1], true);

    AlertCondition alert_condition = alert_condition_create();
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[0]);
    alert_condition_start_new_ored_requirement(alert_condition);
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[1]);
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(false, actual_evaluate_result);
}

TEST(AlertCondition, EvaluateReturnsFalse2ReqsTrueAndFalse)
{
    fake_variable_requirement_set_evaluate_result(variable_requirements[0], true);
    fake_variable_requirement_set_evaluate_result(variable_requirements[1], false);

    AlertCondition alert_condition = alert_condition_create();
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[0]);
    alert_condition_start_new_ored_requirement(alert_condition);
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[1]);
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(false, actual_evaluate_result);
}

TEST(AlertCondition, EvaluateReturnsTrue2ReqsTrueAndTrue)
{
    fake_variable_requirement_set_evaluate_result(variable_requirements[0], true);
    fake_variable_requirement_set_evaluate_result(variable_requirements[1], true);

    AlertCondition alert_condition = alert_condition_create();
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[0]);
    alert_condition_start_new_ored_requirement(alert_condition);
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[1]);
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(true, actual_evaluate_result);
}

TEST(AlertCondition, EvaluateReturnsFalse4Reqs)
{
    /* true <=> ((false OR true) AND (false OR true)) */
    fake_variable_requirement_set_evaluate_result(variable_requirements[0], false);
    fake_variable_requirement_set_evaluate_result(variable_requirements[1], true);
    fake_variable_requirement_set_evaluate_result(variable_requirements[2], false);
    fake_variable_requirement_set_evaluate_result(variable_requirements[3], true);

    AlertCondition alert_condition = alert_condition_create();
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[0]);
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[1]);
    alert_condition_start_new_ored_requirement(alert_condition);
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[2]);
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[3]);
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(true, actual_evaluate_result);
}

TEST(AlertCondition, AddVariableRequirementAssertsMoreReqsThanAllowedAdded)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("!is_num_allowed_requirements_exceeded",
                                        "alert_condition_add_variable_requirement");

    AlertCondition alert_condition = alert_condition_create();
    /* Note the <= : we add one more variable requirement than it is allowed. */
    for (size_t i = 0; i <= CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        alert_condition_add_variable_requirement(alert_condition, variable_requirements[0]);
    }
}

TEST(AlertCondition, EvaluateTrue1ReqTrueStartNewOredReqBeforeAdd)
{
    VariableRequirement variable_requirement = variable_requirements[0];
    bool expected_evaluate_result = true;
    fake_variable_requirement_set_evaluate_result(variable_requirement, expected_evaluate_result);

    AlertCondition alert_condition = alert_condition_create();
    alert_condition_start_new_ored_requirement(alert_condition);
    alert_condition_add_variable_requirement(alert_condition, variable_requirement);
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(expected_evaluate_result, actual_evaluate_result);
}

TEST(AlertCondition, StartNewOredCaledMoreTimesThanNeeded)
{
    fake_variable_requirement_set_evaluate_result(variable_requirements[0], true);
    fake_variable_requirement_set_evaluate_result(variable_requirements[1], false);
    fake_variable_requirement_set_evaluate_result(variable_requirements[2], true);

    AlertCondition alert_condition = alert_condition_create();
    alert_condition_start_new_ored_requirement(alert_condition);
    alert_condition_start_new_ored_requirement(alert_condition);
    alert_condition_start_new_ored_requirement(alert_condition);
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[0]);
    alert_condition_start_new_ored_requirement(alert_condition);
    alert_condition_start_new_ored_requirement(alert_condition);
    alert_condition_start_new_ored_requirement(alert_condition);
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[1]);
    alert_condition_start_new_ored_requirement(alert_condition);
    alert_condition_start_new_ored_requirement(alert_condition);
    alert_condition_start_new_ored_requirement(alert_condition);
    alert_condition_add_variable_requirement(alert_condition, variable_requirements[2]);
    alert_condition_start_new_ored_requirement(alert_condition);
    alert_condition_start_new_ored_requirement(alert_condition);
    alert_condition_start_new_ored_requirement(alert_condition);
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(false, actual_evaluate_result);
}

TEST(AlertCondition, StartNewOredCaledMoreTimesThanNeededMaxNumVariableReqs)
{
    /* Make sure the test contains enough variable requirements to test adding max num variables requirements */
    EAS_ASSERT(TEST_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS >=
               CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS);

    for (size_t i = 0; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        fake_variable_requirement_set_evaluate_result(variable_requirements[i], false);
    }

    AlertCondition alert_condition = alert_condition_create();
    /* start_new_ored_requirement is unnecessarily called once before the first requirement is added, twice in between
     * adding subsequent requirements, and once after the last requirement. We are testing that excessive calling of
     * this function does not affect with the result.*/
    for (size_t i = 0; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        alert_condition_start_new_ored_requirement(alert_condition);
        alert_condition_add_variable_requirement(alert_condition, variable_requirements[i]);
        alert_condition_start_new_ored_requirement(alert_condition);
    }
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(false, actual_evaluate_result);
}

TEST(AlertCondition, EvaluateMaxNumVariableReqsAndedAllFalse)
{
    /* Make sure the test contains enough variable requirements to test adding max num variables requirements */
    EAS_ASSERT(TEST_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS >=
               CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS);

    for (size_t i = 0; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        fake_variable_requirement_set_evaluate_result(variable_requirements[i], false);
    }

    AlertCondition alert_condition = alert_condition_create();
    for (size_t i = 0; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        alert_condition_start_new_ored_requirement(alert_condition);
        alert_condition_add_variable_requirement(alert_condition, variable_requirements[i]);
    }
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(false, actual_evaluate_result);
}

TEST(AlertCondition, EvaluateMaxNumVariableReqsAndedAllTrue)
{
    /* Make sure the test contains enough variable requirements to test adding max num variables requirements */
    EAS_ASSERT(TEST_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS >=
               CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS);

    for (size_t i = 0; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        fake_variable_requirement_set_evaluate_result(variable_requirements[i], true);
    }

    AlertCondition alert_condition = alert_condition_create();
    for (size_t i = 0; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        alert_condition_start_new_ored_requirement(alert_condition);
        alert_condition_add_variable_requirement(alert_condition, variable_requirements[i]);
    }
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(true, actual_evaluate_result);
}

TEST(AlertCondition, EvaluateMaxNumVariableReqsAndedOnlyOneFalse)
{
    /* Make sure the test contains enough variable requirements to test adding max num variables requirements */
    EAS_ASSERT(TEST_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS >=
               CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS);

    for (size_t i = 0; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS - 1; i++) {
        fake_variable_requirement_set_evaluate_result(variable_requirements[i], true);
    }
    fake_variable_requirement_set_evaluate_result(
        variable_requirements[CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS - 1], false);

    AlertCondition alert_condition = alert_condition_create();
    for (size_t i = 0; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        alert_condition_start_new_ored_requirement(alert_condition);
        alert_condition_add_variable_requirement(alert_condition, variable_requirements[i]);
    }
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(false, actual_evaluate_result);
}

TEST(AlertCondition, EvaluateMaxNumVariableReqsAndedHalfFalse)
{
    /* Make sure the test contains enough variable requirements to test adding max num variables requirements */
    EAS_ASSERT(TEST_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS >=
               CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS);

    bool evaluate_result = false;
    for (size_t i = 0; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        fake_variable_requirement_set_evaluate_result(variable_requirements[i], evaluate_result);
        evaluate_result = !evaluate_result;
    }

    AlertCondition alert_condition = alert_condition_create();
    for (size_t i = 0; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        alert_condition_start_new_ored_requirement(alert_condition);
        alert_condition_add_variable_requirement(alert_condition, variable_requirements[i]);
    }
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(false, actual_evaluate_result);
}

TEST(AlertCondition, EvaluateMaxNumVariableReqsOredAllFalse)
{
    /* Make sure the test contains enough variable requirements to test adding max num variables requirements */
    EAS_ASSERT(TEST_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS >=
               CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS);

    for (size_t i = 0; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        fake_variable_requirement_set_evaluate_result(variable_requirements[i], false);
    }

    AlertCondition alert_condition = alert_condition_create();
    for (size_t i = 0; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        alert_condition_add_variable_requirement(alert_condition, variable_requirements[i]);
    }
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(false, actual_evaluate_result);
}

TEST(AlertCondition, EvaluateMaxNumVariableReqsOredAllTrue)
{
    /* Make sure the test contains enough variable requirements to test adding max num variables requirements */
    EAS_ASSERT(TEST_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS >=
               CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS);

    for (size_t i = 0; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        fake_variable_requirement_set_evaluate_result(variable_requirements[i], true);
    }

    AlertCondition alert_condition = alert_condition_create();
    for (size_t i = 0; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        alert_condition_add_variable_requirement(alert_condition, variable_requirements[i]);
    }
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(true, actual_evaluate_result);
}

/* The purpose of this test is to check that calling start_new_ored_requirement before adding variable requirements does
 * not interpret any requirements as ANDed, if all of them are supposed to be one big ORed requirement. We set the first
 * requirement to evaluate as false, so in case it does get misinterpreted as ANDed, the overall evaluation result will
 * be false while we are expecting it to be true. */
TEST(AlertCondition, StartNewOredBeforehandDoesNotInterpretTheFirstReqAsAnded)
{
    /* Make sure the test contains enough variable requirements to test adding max num variables requirements */
    EAS_ASSERT(TEST_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS >=
               CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS);

    fake_variable_requirement_set_evaluate_result(variable_requirements[0], false);
    for (size_t i = 1; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        fake_variable_requirement_set_evaluate_result(variable_requirements[i], true);
    }

    AlertCondition alert_condition = alert_condition_create();
    alert_condition_start_new_ored_requirement(alert_condition);
    for (size_t i = 0; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        alert_condition_add_variable_requirement(alert_condition, variable_requirements[i]);
    }
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(true, actual_evaluate_result);
}

TEST(AlertCondition, EvaluateMaxNumVariableReqsOredHalfFalse)
{
    /* Make sure the test contains enough variable requirements to test adding max num variables requirements */
    EAS_ASSERT(TEST_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS >=
               CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS);

    bool evaluate_result = false;
    for (size_t i = 0; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        fake_variable_requirement_set_evaluate_result(variable_requirements[i], evaluate_result);
        evaluate_result = !evaluate_result;
    }

    AlertCondition alert_condition = alert_condition_create();
    for (size_t i = 0; i < CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS; i++) {
        alert_condition_add_variable_requirement(alert_condition, variable_requirements[i]);
    }
    bool actual_evaluate_result = alert_condition_evaluate(alert_condition);

    CHECK_EQUAL(true, actual_evaluate_result);
}
