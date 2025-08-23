#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport_c.h"

/* We are using the C CppUTest interface instead of C++, because this header would not compile under C++. */
#include "pressure_requirement.h"

static VariableRequirement pressure_requirement;

static void test_evaluate(Pressure current_pressure, uint8_t operator, Pressure requirement_value, bool expected_result)
{
    mock_c()->expectOneCall("current_pressure_get")->andReturnUnsignedIntValue(current_pressure);

    pressure_requirement = pressure_requirement_create(0, operator, requirement_value);
    bool result = variable_requirement_evaluate(pressure_requirement);
    CHECK_EQUAL_C_BOOL(expected_result, result);
}

TEST_GROUP_C_TEARDOWN(PressureRequirement)
{
    variable_requirement_destroy(pressure_requirement);
}

TEST_C(PressureRequirement, evaluateReturnsTrueOperatorGEQValueGreater)
{
    test_evaluate(10300, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 10000, true);
}

TEST_C(PressureRequirement, evaluateReturnsFalseOperatorGEQValueLess)
{
    test_evaluate(2000, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 2500, false);
}

TEST_C(PressureRequirement, evaluateReturnsTrueOperatorGEQValueEqual)
{
    test_evaluate(10000, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 10000, true);
}

TEST_C(PressureRequirement, evaluateReturnsTrueOperatorLEQValueLess)
{
    test_evaluate(8887, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 8888, true);
}

TEST_C(PressureRequirement, evaluateReturnsTrueOperatorLEQValueEqual)
{
    test_evaluate(2200, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 2200, true);
}

TEST_C(PressureRequirement, evaluateReturnsFalseOperatorLEQValueGreater)
{
    test_evaluate(10000, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 5000, false);
}

TEST_C(PressureRequirement, evaluateReturnsTrueOperatorGEQBothValuesEqualZero)
{
    test_evaluate(0, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 0, true);
}

TEST_C(PressureRequirement, evaluateReturnsTrueOperatorLEQBothValuesEqualZero)
{
    test_evaluate(0, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 0, true);
}

/* The get_alert_id() tests below belong in testing pressure_requirement and not variable_requirement, because it
 * is the responsibility of pressure_requirement_create() to propagate the alert_id to the base class. We are testing
 * that link here. */
TEST_C(PressureRequirement, getAlertIdReturnsAlertId1PassedToCreate)
{
    uint8_t expected_alert_id = 1;
    pressure_requirement = pressure_requirement_create(expected_alert_id, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 200);
    uint8_t actual_alert_id = variable_requirement_get_alert_id(pressure_requirement);
    CHECK_EQUAL_C_UINT(expected_alert_id, actual_alert_id);
}

TEST_C(PressureRequirement, getAlertIdReturnsAlertId2PassedToCreate)
{
    uint8_t expected_alert_id = 2;
    pressure_requirement = pressure_requirement_create(expected_alert_id, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 10000);
    uint8_t actual_alert_id = variable_requirement_get_alert_id(pressure_requirement);
    CHECK_EQUAL_C_UINT(expected_alert_id, actual_alert_id);
}
