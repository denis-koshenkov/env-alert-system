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
