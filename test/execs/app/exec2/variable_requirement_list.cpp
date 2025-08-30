#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP_C_WRAPPER(VariableRequirementList)
{
    TEST_GROUP_C_SETUP_WRAPPER(VariableRequirementList);
    TEST_GROUP_C_TEARDOWN_WRAPPER(VariableRequirementList);
};

TEST_C_WRAPPER(VariableRequirementList, ListIsEmptyAfterCreate);
TEST_C_WRAPPER(VariableRequirementList, ListContainsOneVarWhenOneVarAdded);
TEST_C_WRAPPER(VariableRequirementList, ListContainsThreeVarsWhenThreeVarsAdded);
TEST_C_WRAPPER(VariableRequirementList, RemoveAllVarsOfAlertEmptyList);
TEST_C_WRAPPER(VariableRequirementList, RemoveAllVarsOfAlertRemovesTheOnlyRequirement);
TEST_C_WRAPPER(VariableRequirementList, RemoveAllVarsOfAlertKeepsTheOnlyRequirement);
