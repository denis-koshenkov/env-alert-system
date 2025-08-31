#include "CppUTest/TestHarness.h"
#include "CppUTestExt/OrderedTest.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "alert_conditions.h"

TEST_GROUP(AlertConditions){};

TEST_ORDERED(AlertConditions, GetAssertsIfCalledBeforeSet, 0)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("0", "alert_conditions_get_alert_condition");

    AlertCondition alert_condition_0 = alert_conditions_get_alert_condition(0);
}
