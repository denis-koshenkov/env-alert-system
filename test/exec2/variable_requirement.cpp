#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "variable_requirement.h"
#include "mocks/variable_requirements/vtable_null_requirement.h"
#include "mocks/variable_requirements/evaluate_null_requirement.h"

/* ------------------------------------------ VariableRequirement test group ------------------------------------ */

TEST_GROUP(VariableRequirement){};

TEST(VariableRequirement, evaluateRaisesAssertIfCalledWithNullPointer)
{
    TestAssertPlugin::expectAssertion("variable_requirement");
    variable_requirement_evaluate(NULL);
}

/* ------------------------------------------ VariableRequirementVtableNull test group -------------------------- */

/* VariableRequirementVtableNull is a different test group, because we need to call vtable_null_requirement_destroy() in
 * teardown. If we put it in the test body, it will not be executed, because assert fires and the test ends prematurely.
 * But we should not call vtable_null_requirement_destroy() for the VariableRequirement test group tests. */
static VariableRequirement vtable_null_requirement;

// clang-format off
TEST_GROUP(VariableRequirementVtableNull)
{
    void setup()
    {
        vtable_null_requirement = vtable_null_requirement_create();
    }

    void teardown()
    {
        vtable_null_requirement_destroy(vtable_null_requirement);
    }
};
// clang-format on

TEST(VariableRequirementVtableNull, evaluateRaisesAssertIfVtableIsNull)
{
    TestAssertPlugin::expectAssertion("variable_requirement->vtable");
    variable_requirement_evaluate(vtable_null_requirement);
}

/* ------------------------------------------ VariableRequirementEvaluateNull test group -------------------------- */

/* Same logic applies to VariableRequirementEvaluateNull test group as to the VariableRequirementVtableNull test
 * group. */
static VariableRequirement evaluate_null_requirement;

// clang-format off
TEST_GROUP(VariableRequirementEvaluateNull)
{
    void setup()
    {
        evaluate_null_requirement = evaluate_null_requirement_create();
    }

    void teardown()
    {
        evaluate_null_requirement_destroy(evaluate_null_requirement);
    }
};
// clang-format on

TEST(VariableRequirementEvaluateNull, evaluateRaisesAssertIfEvaluateIsNull)
{
    TestAssertPlugin::expectAssertion("variable_requirement->vtable->evaluate");
    variable_requirement_evaluate(evaluate_null_requirement);
}
