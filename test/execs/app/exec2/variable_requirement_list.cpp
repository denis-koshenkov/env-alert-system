#include "CppUTest/TestHarness.h"

#include "variable_requirement_list.h"

static bool for_each_cb_called = false;
static void for_each_cb(VariableRequirement variable_requirement)
{
    for_each_cb_called = true;
}

TEST_GROUP(VariableRequirementList){};

TEST(VariableRequirementList, ListIsEmptyAfterCreate)
{
    VariableRequirementList list = variable_requirement_list_create();
    variable_requirement_list_for_each(list, for_each_cb);
    CHECK_FALSE(for_each_cb_called);
}
