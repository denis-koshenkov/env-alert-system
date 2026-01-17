#include "CppUTest/TestHarness.h"
#include "CppUTestExt/OrderedTest.h"
#include "CppUTestExt/TestAssertPlugin.h"
#include "CppUTestExt/MockSupport.h"

#include "alert_conditions.h"
#include "config.h"
#include "eas_assert.h"

static AlertCondition expected_instances[CONFIG_ALERT_CONDITIONS_NUM_INSTANCES_TO_CREATE];

static AlertCondition map_expected_instance_index_to_instance(size_t index)
{
    /* Random formula. No need to create actual instances since we only need to test that the expected instances are
     * returned by alert_conditions_get_instance, we do not need to interact with the instances themselves. */
    return (AlertCondition)((index * 2) + 3);
}

static void initialize_expected_instances()
{
    for (size_t i = 0; i < CONFIG_ALERT_CONDITIONS_NUM_INSTANCES_TO_CREATE; i++) {
        expected_instances[i] = map_expected_instance_index_to_instance(i);
    }
}

TEST_GROUP(AlertConditions){};

/* Ordered test group 0. These tests are executed before ordered test group 1, because we want to test the state before
 * alert_conditions_create_instances is called, which happens in a group 1 test. */

TEST_ORDERED(AlertConditions, GetAssertsIfCalledBeforeCreateInstancesId0, 0)
{
    uint8_t alert_id = 0;
    EAS_ASSERT(alert_id < CONFIG_ALERT_CONDITIONS_NUM_INSTANCES_TO_CREATE);
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("alert_condition", "alert_conditions_get_alert_condition");

    AlertCondition alert_condition = alert_conditions_get_alert_condition(alert_id);
}

TEST_ORDERED(AlertConditions, GetAssertsIfCalledBeforeCreateInstancesId2, 0)
{
    uint8_t alert_id = 2;
    EAS_ASSERT(alert_id < CONFIG_ALERT_CONDITIONS_NUM_INSTANCES_TO_CREATE);
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("alert_condition", "alert_conditions_get_alert_condition");

    AlertCondition alert_condition = alert_conditions_get_alert_condition(alert_id);
}

/* Ordered test group 1 */

TEST_ORDERED(AlertConditions, GetReturnsCreatedInstances, 1)
{
    initialize_expected_instances();

    AlertCondition instance_to_return;
    for (size_t i = 0; i < CONFIG_ALERT_CONDITIONS_NUM_INSTANCES_TO_CREATE; i++) {
        instance_to_return = map_expected_instance_index_to_instance(i);
        mock().expectOneCall("alert_condition_create").andReturnValue(instance_to_return);
    }

    alert_conditions_create_instances();

    AlertCondition actual_instance;
    AlertCondition expected_instance;
    for (size_t i = 0; i < CONFIG_ALERT_CONDITIONS_NUM_INSTANCES_TO_CREATE; i++) {
        actual_instance = alert_conditions_get_alert_condition(i);
        expected_instance = map_expected_instance_index_to_instance(i);

        CHECK_EQUAL(expected_instance, actual_instance);
    }
}

/* Ordered test group 2. Test the state after alert_conditions_create_instances is called, which happens in ordered test
 * group 1. */

TEST_ORDERED(AlertConditions, GetAssertsInvalidAlertId, 2)
{
    uint8_t invalid_alert_id = CONFIG_ALERT_CONDITIONS_NUM_INSTANCES_TO_CREATE;
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("is_valid_alert_id", "alert_conditions_get_alert_condition");

    AlertCondition alert_condition = alert_conditions_get_alert_condition(invalid_alert_id);
}

/* alert_conditions_create_instances is already called as a part of ordered test group 1, so we only need to call it
 * once in this test to trigger an assert. */
TEST_ORDERED(AlertConditions, CreateInstancesAssertsIfCalledAgain, 2)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("0", "alert_conditions_create_instances");

    alert_conditions_create_instances();
}
