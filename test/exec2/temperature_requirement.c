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
