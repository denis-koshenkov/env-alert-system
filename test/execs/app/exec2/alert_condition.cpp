#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "alert_condition.h"

TEST_GROUP(AlertCondition){};

TEST(AlertCondition, EvaluateAssertsNoVariableRequirementsAdded)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("0", "alert_condition_evaluate");

    AlertCondition alert_condition = alert_condition_create();
    bool unused = alert_condition_evaluate(alert_condition);
}
