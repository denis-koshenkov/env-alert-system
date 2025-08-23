#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport_c.h"
#include "CppUTestExt/TestAssertPlugin_c.h"

/* We are using the C CppUTest interface instead of C++, because this header would not compile under C++. */
#include "light_intensity_requirement.h"

static VariableRequirement light_intensity_requirement;

static void test_evaluate(LightIntensity current_light_intensity, uint8_t operator, LightIntensity requirement_value,
                          bool expected_result)
{
    mock_c()->expectOneCall("current_light_intensity_get")->andReturnUnsignedIntValue(current_light_intensity);

    light_intensity_requirement = light_intensity_requirement_create(0, operator, requirement_value);
    bool result = variable_requirement_evaluate(light_intensity_requirement);
    CHECK_EQUAL_C_BOOL(expected_result, result);
}

TEST_GROUP_C_TEARDOWN(LightIntensityRequirement)
{
    variable_requirement_destroy(light_intensity_requirement);
}

TEST_C(LightIntensityRequirement, evaluateReturnsTrueOperatorGEQValueGreater)
{
    test_evaluate(10300, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 10000, true);
}

TEST_C(LightIntensityRequirement, evaluateReturnsFalseOperatorGEQValueLess)
{
    test_evaluate(2000, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 2500, false);
}

TEST_C(LightIntensityRequirement, evaluateReturnsTrueOperatorGEQValueEqual)
{
    test_evaluate(10000, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 10000, true);
}

TEST_C(LightIntensityRequirement, evaluateReturnsTrueOperatorLEQValueLess)
{
    test_evaluate(1, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 2, true);
}

TEST_C(LightIntensityRequirement, evaluateReturnsTrueOperatorLEQValueEqual)
{
    test_evaluate(2200, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 2200, true);
}

TEST_C(LightIntensityRequirement, evaluateReturnsFalseOperatorLEQValueGreater)
{
    test_evaluate(10000, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 5000, false);
}

TEST_C(LightIntensityRequirement, evaluateReturnsTrueOperatorGEQBothValuesEqualZero)
{
    test_evaluate(0, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 0, true);
}

TEST_C(LightIntensityRequirement, evaluateReturnsTrueOperatorLEQBothValuesEqualZero)
{
    test_evaluate(0, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 0, true);
}

/* The get_alert_id() tests below belong in testing light_intensity_requirement and not variable_requirement, because it
 * is the responsibility of light_intensity_requirement_create() to propagate the alert_id to the base class. We are
 * testing that link here. */
TEST_C(LightIntensityRequirement, getAlertIdReturnsAlertId3PassedToCreate)
{
    uint8_t expected_alert_id = 3;
    light_intensity_requirement =
        light_intensity_requirement_create(expected_alert_id, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 200);
    uint8_t actual_alert_id = variable_requirement_get_alert_id(light_intensity_requirement);
    CHECK_EQUAL_C_UINT(expected_alert_id, actual_alert_id);
}

TEST_C(LightIntensityRequirement, getAlertIdReturnsAlertId8PassedToCreate)
{
    uint8_t expected_alert_id = 8;
    light_intensity_requirement =
        light_intensity_requirement_create(expected_alert_id, VARIABLE_REQUIREMENT_OPERATOR_LEQ, 10000);
    uint8_t actual_alert_id = variable_requirement_get_alert_id(light_intensity_requirement);
    CHECK_EQUAL_C_UINT(expected_alert_id, actual_alert_id);
}

TEST_C(LightIntensityRequirement, createRaisesAssertIfMemoryAllocationFailed)
{
    light_intensity_requirement = light_intensity_requirement_create(0, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 0);
    /* Since we already called light_intensity_requirement_create(), and mock variable requirement allocator can only
     * allocate one variable requirement instance at a time, variable_requirement_allocator_alloc() will return NULL
     * when it is called again as a part of light_intensity_requirement_create(). We expect the implementation of
     * light_intensity_requirement_create() to detect this and raise an assert. */
    TEST_ASSERT_PLUGIN_C_EXPECT_ASSERTION("self", "light_intensity_requirement_create");
    light_intensity_requirement_create(0, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 0);
}
