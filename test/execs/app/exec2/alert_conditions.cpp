#include "CppUTest/TestHarness.h"
#include "CppUTestExt/OrderedTest.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "alert_conditions.h"

TEST_GROUP(AlertConditions){};

TEST_ORDERED(AlertConditions, GetAssertsIfCalledBeforeSet, 0)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("alert_condition_instance", "alert_conditions_get_alert_condition");

    AlertCondition alert_condition_0 = alert_conditions_get_alert_condition(0);
}

TEST_ORDERED(AlertConditions, SetAssertsIfConditionIsNull, 1)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("alert_condition", "alert_conditions_set_alert_condition");

    alert_conditions_set_alert_condition(0, NULL);
}

TEST_ORDERED(AlertConditions, GetWhatWeSetAlertId0, 1)
{
    /* We are only checking that the same instance is returned - no need to use a real instance */
    AlertCondition expected_alert_condition = (AlertCondition)0x4926;

    alert_conditions_set_alert_condition(0, expected_alert_condition);
    AlertCondition actual_alert_condition = alert_conditions_get_alert_condition(0);
    CHECK_EQUAL(expected_alert_condition, actual_alert_condition);
}
