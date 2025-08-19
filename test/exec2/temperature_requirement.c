#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport_c.h"

/* We are using the C CppUTest interface instead of C++, because this header would not compile under C++. */
#include "temperature_requirement.h"

static VariableRequirement temperature_requirement;

static void test_evaluate(Temperature current_temperature, VariableRequirementOperator operator,
                          Temperature requirement_value, bool expected_result)
{
    mock_c()->expectOneCall("current_temperature_get")->andReturnUnsignedIntValue(current_temperature);

    temperature_requirement = temperature_requirement_create(0, operator, requirement_value);
    bool result = variable_requirement_evaluate(temperature_requirement);
    CHECK_EQUAL_C_BOOL(expected_result, result);
}

TEST_GROUP_C_TEARDOWN(TemperatureRequirement)
{
    temperature_requirement_destroy(temperature_requirement);
}

TEST_C(TemperatureRequirement, evaluateReturnsTrueOperatorGEQValueGreater)
{
    test_evaluate(30, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 22, true);
}

TEST_C(TemperatureRequirement, evaluateReturnsFalseOperatorGEQValueLess)
{
    test_evaluate(20, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 25, false);
}

TEST_C(TemperatureRequirement, evaluateReturnsTrueOperatorGEQValueEqual)
{
    test_evaluate(100, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 100, true);
}

TEST_C(TemperatureRequirement, evaluateReturnsTrueOperatorLEQValueLess)
{
    test_evaluate(87, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 88, true);
}

TEST_C(TemperatureRequirement, evaluateReturnsTrueOperatorLEQValueEqual)
{
    test_evaluate(220, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 220, true);
}

TEST_C(TemperatureRequirement, evaluateReturnsFalseOperatorLEQValueGreater)
{
    test_evaluate(100, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 50, false);
}

TEST_C(TemperatureRequirement, evaluateReturnsTrueOperatorGEQBothValuesEqualZero)
{
    test_evaluate(0, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 0, true);
}

TEST_C(TemperatureRequirement, evaluateReturnsTrueOperatorLEQBothValuesEqualZero)
{
    test_evaluate(0, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 0, true);
}

TEST_C(TemperatureRequirement, evaluateReturnsTrueOperatorGEQValueGreaterBothNegative)
{
    test_evaluate(-200, VARIABLE_REQUIREMENT_OPERATOR_GEQ, -300, true);
}

TEST_C(TemperatureRequirement, evaluateReturnsFalseOperatorGEQValueLessBothNegative)
{
    test_evaluate(-11, VARIABLE_REQUIREMENT_OPERATOR_GEQ, -1, false);
}

TEST_C(TemperatureRequirement, evaluateReturnsTrueOperatorGEQValueGreaterOneNegative)
{
    test_evaluate(10, VARIABLE_REQUIREMENT_OPERATOR_GEQ, -10, true);
}

TEST_C(TemperatureRequirement, evaluateReturnsFalseOperatorLEQValueGreaterBothNegative)
{
    test_evaluate(-200, VARIABLE_REQUIREMENT_OPERATOR_LEQ, -300, false);
}

TEST_C(TemperatureRequirement, evaluateReturnsTrueOperatorLEQValueLessBothNegative)
{
    test_evaluate(-101, VARIABLE_REQUIREMENT_OPERATOR_LEQ, -100, true);
}

TEST_C(TemperatureRequirement, evaluateReturnsFalseOperatorLEQValueGreaterOneNegative)
{
    test_evaluate(76, VARIABLE_REQUIREMENT_OPERATOR_LEQ, -2, false);
}

/* The get_alert_id() tests below belong in testing temperature_requirement and not variable_requirement, because it
 * is the responsibility of temperature_requirement_create() to propagate the alert_id to the base class. We are testing
 * that link here. */
TEST_C(TemperatureRequirement, getAlertIdReturnsAlertId1PassedToCreate)
{
    uint8_t expected_alert_id = 1;
    temperature_requirement = temperature_requirement_create(expected_alert_id, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 200);
    uint8_t actual_alert_id = variable_requirement_get_alert_id(temperature_requirement);
    CHECK_EQUAL_C_UINT(expected_alert_id, actual_alert_id);
}

TEST_C(TemperatureRequirement, getAlertIdReturnsAlertId2PassedToCreate)
{
    uint8_t expected_alert_id = 2;
    temperature_requirement =
        temperature_requirement_create(expected_alert_id, VARIABLE_REQUIREMENT_OPERATOR_LEQ, -200);
    uint8_t actual_alert_id = variable_requirement_get_alert_id(temperature_requirement);
    CHECK_EQUAL_C_UINT(expected_alert_id, actual_alert_id);
}
