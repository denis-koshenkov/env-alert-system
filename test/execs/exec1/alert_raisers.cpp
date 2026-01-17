#include "CppUTest/TestHarness.h"
#include "CppUTestExt/OrderedTest.h"
#include "CppUTestExt/TestAssertPlugin.h"
#include "CppUTestExt/MockSupport.h"

#include "alert_raisers.h"
#include "config.h"
#include "eas_assert.h"

static AlertRaiser expected_instances[CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE];

static AlertRaiser map_expected_instance_index_to_instance(size_t index)
{
    /* Random formula. No need to create actual instances since we only need to test that the expected instances are
     * returned by alert_raisers_get_instance, we do not need to interact with the instances themselves. */
    return (AlertRaiser)((index * 3) + 5);
}

static void initialize_expected_instances()
{
    for (size_t i = 0; i < CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE; i++) {
        expected_instances[i] = map_expected_instance_index_to_instance(i);
    }
}

TEST_GROUP(AlertRaisers){};

/* Ordered test group 0. These tests are executed before ordered test group 1, because we want to test the state before
 * alert_raisers_create_instances is called, which happens in a group 1 test. */

TEST_ORDERED(AlertRaisers, GetAssertsIfCalledBeforeCreateInstancesId0, 0)
{
    uint8_t alert_id = 0;
    EAS_ASSERT(alert_id < CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE);
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("alert_raiser", "alert_raisers_get_alert_raiser");

    AlertRaiser alert_raiser = alert_raisers_get_alert_raiser(alert_id);
}

TEST_ORDERED(AlertRaisers, GetAssertsIfCalledBeforeCreateInstancesId2, 0)
{
    uint8_t alert_id = 2;
    EAS_ASSERT(alert_id < CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE);
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("alert_raiser", "alert_raisers_get_alert_raiser");

    AlertRaiser alert_raiser = alert_raisers_get_alert_raiser(alert_id);
}

/* Ordered test group 1 */

TEST_ORDERED(AlertRaisers, GetReturnsCreatedInstances, 1)
{
    initialize_expected_instances();

    AlertRaiser instance_to_return;
    for (size_t i = 0; i < CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE; i++) {
        instance_to_return = map_expected_instance_index_to_instance(i);
        mock().expectOneCall("alert_raiser_create").andReturnValue(instance_to_return);
    }

    alert_raisers_create_instances();

    AlertRaiser actual_instance;
    AlertRaiser expected_instance;
    for (size_t i = 0; i < CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE; i++) {
        actual_instance = alert_raisers_get_alert_raiser(i);
        expected_instance = map_expected_instance_index_to_instance(i);

        CHECK_EQUAL(expected_instance, actual_instance);
    }
}

/* Ordered test group 2. Test the state after alert_raisers_create_instances is called, which happens in ordered test
 * group 1. */

TEST_ORDERED(AlertRaisers, GetAssertsInvalidAlertId, 2)
{
    uint8_t invalid_alert_id = CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE;
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("is_valid_alert_id", "alert_raisers_get_alert_raiser");

    AlertRaiser alert_raiser = alert_raisers_get_alert_raiser(invalid_alert_id);
}

/* alert_raisers_create_instances is already called as a part of ordered test group 1, so we only need to call it
 * once in this test to trigger an assert. */
TEST_ORDERED(AlertRaisers, CreateInstancesAssertsIfCalledAgain, 2)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("0", "alert_raisers_create_instances");

    alert_raisers_create_instances();
}
