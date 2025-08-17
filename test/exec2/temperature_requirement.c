#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport_c.h"

/* We are using the C CppUTest interface instead of C++, because this header would not compile under C++. */
#include "temperature_requirement.h"

TEST_C(TemperatureRequirement, evaluateReturnsTrueOperatorGEQValueGreater)
{
    Temperature current_temperature_value = 30;
    mock_c()->expectOneCall("current_temperature_get")->andReturnUnsignedIntValue(current_temperature_value);

    VariableRequirement temperature_requirement =
        temperature_requirement_create(0, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 22);
    bool result = variable_requirement_evaluate(temperature_requirement);
    CHECK_C(result);

    temperature_requirement_destroy(temperature_requirement);
}
