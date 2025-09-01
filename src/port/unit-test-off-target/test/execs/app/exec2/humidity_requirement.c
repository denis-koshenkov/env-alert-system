#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport_c.h"
#include "CppUTestExt/TestAssertPlugin_c.h"
#include "fake_variable_requirement_allocator.h"

/* We are using the C CppUTest interface instead of C++, because this header would not compile under C++. */
#include "humidity_requirement.h"

static VariableRequirement humidity_requirement;
static void *requirement_buffer;

static void test_evaluate(Humidity current_humidity, uint8_t operator, Humidity requirement_value, bool expected_result)
{
    mock_c()->expectOneCall("variable_requirement_allocator_alloc")->andReturnPointerValue(requirement_buffer);
    mock_c()->expectOneCall("current_humidity_get")->andReturnUnsignedIntValue(current_humidity);
    mock_c()->expectOneCall("variable_requirement_allocator_free")->withPointerParameters("buf", requirement_buffer);

    humidity_requirement = humidity_requirement_create(0, operator, requirement_value);
    bool result = variable_requirement_evaluate(humidity_requirement);
    CHECK_EQUAL_C_BOOL(expected_result, result);

    /* Clean up */
    variable_requirement_destroy(humidity_requirement);
}

TEST_GROUP_C_SETUP(HumidityRequirement)
{
    requirement_buffer = fake_variable_requirement_allocator_alloc();
}

TEST_GROUP_C_TEARDOWN(HumidityRequirement)
{
    fake_variable_requirement_allocator_free(requirement_buffer);
}

TEST_C(HumidityRequirement, evaluateReturnsTrueOperatorGEQValueGreater)
{
    test_evaluate(400, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 300, true);
}

TEST_C(HumidityRequirement, evaluateReturnsFalseOperatorGEQValueLess)
{
    test_evaluate(200, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 250, false);
}

TEST_C(HumidityRequirement, evaluateReturnsTrueOperatorGEQValueEqual)
{
    test_evaluate(1000, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 1000, true);
}

TEST_C(HumidityRequirement, evaluateReturnsTrueOperatorLEQValueLess)
{
    test_evaluate(345, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 346, true);
}

TEST_C(HumidityRequirement, evaluateReturnsTrueOperatorLEQValueEqual)
{
    test_evaluate(499, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 499, true);
}

TEST_C(HumidityRequirement, evaluateReturnsFalseOperatorLEQValueGreater)
{
    test_evaluate(1000, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 500, false);
}

TEST_C(HumidityRequirement, evaluateReturnsTrueOperatorGEQBothValuesEqualZero)
{
    test_evaluate(0, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 0, true);
}

TEST_C(HumidityRequirement, evaluateReturnsTrueOperatorLEQBothValuesEqualZero)
{
    test_evaluate(0, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 0, true);
}

/* The get_alert_id() tests below belong in testing humidity_requirement and not variable_requirement, because it
 * is the responsibility of humidity_requirement_create() to propagate the alert_id to the base class. We are testing
 * that link here. */
TEST_C(HumidityRequirement, getAlertIdReturnsAlertId5PassedToCreate)
{
    mock_c()->expectOneCall("variable_requirement_allocator_alloc")->andReturnPointerValue(requirement_buffer);
    mock_c()->expectOneCall("variable_requirement_allocator_free")->withPointerParameters("buf", requirement_buffer);
    uint8_t expected_alert_id = 5;

    humidity_requirement = humidity_requirement_create(expected_alert_id, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 200);
    uint8_t actual_alert_id = variable_requirement_get_alert_id(humidity_requirement);

    CHECK_EQUAL_C_UINT(expected_alert_id, actual_alert_id);

    /* Clean up */
    variable_requirement_destroy(humidity_requirement);
}

TEST_C(HumidityRequirement, getAlertIdReturnsAlertId6PassedToCreate)
{
    mock_c()->expectOneCall("variable_requirement_allocator_alloc")->andReturnPointerValue(requirement_buffer);
    mock_c()->expectOneCall("variable_requirement_allocator_free")->withPointerParameters("buf", requirement_buffer);
    uint8_t expected_alert_id = 6;

    humidity_requirement = humidity_requirement_create(expected_alert_id, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 300);
    uint8_t actual_alert_id = variable_requirement_get_alert_id(humidity_requirement);

    CHECK_EQUAL_C_UINT(expected_alert_id, actual_alert_id);

    /* Clean up */
    variable_requirement_destroy(humidity_requirement);
}

TEST_C(HumidityRequirement, createRaisesAssertIfMemoryAllocationFailed)
{
    mock_c()->expectOneCall("variable_requirement_allocator_alloc")->andReturnPointerValue((void *)NULL);
    TEST_ASSERT_PLUGIN_C_EXPECT_ASSERTION("self", "humidity_requirement_create");

    humidity_requirement_create(0, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 0);
}
