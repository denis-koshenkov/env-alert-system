#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "temperature_requirement_list.h"

TEST_GROUP(TemperatureRequirementList){};

/* Tests that variable_requirement_list_create() is called only once. Also tests that all wrapper functions - add,
 * for_each, remove_all_for_alert - use the instance returned by variable_requirement_list_create() and correctly
 * propagate function calls to its variable_requirement_list counterparts.
 *
 * It is all in one test because the order of execution of different tests is not guaranteed, so we would have no way of
 * knowing which test would actually call variable_requirement_list_create(), since it only gets called when a
 * temperature_requirement_list function gets called for the first time. */
TEST(TemperatureRequirementList, CreateIsCalledOnlyOnceAndWrappersPropagateCalls)
{
    /* We are only verifying that temperature_requirement functions propagate the function arguments to
     * variable_requirement_list functions, so it is not necessary to create proper instances of VariableRequirement
     * or VariableRequirementListForEachCb. */
    void *variable_requirement_list_instance_address = (void *)0xFF5A;
    VariableRequirement temperature_requirement = (VariableRequirement)0x425A;
    VariableRequirementListForEachCb for_each_cb = (VariableRequirementListForEachCb)0x5A5A;

    uint8_t alert_id = 6;

    mock().expectOneCall("variable_requirement_list_create").andReturnValue(variable_requirement_list_instance_address);
    mock()
        .expectOneCall("variable_requirement_list_add")
        .withParameter("self", variable_requirement_list_instance_address)
        .withParameter("variable_requirement", temperature_requirement);
    mock()
        .expectOneCall("variable_requirement_list_for_each")
        .withParameter("self", variable_requirement_list_instance_address)
        .withParameterOfType("VariableRequirementListForEachCb", "cb", (const void *)for_each_cb);
    mock()
        .expectOneCall("variable_requirement_list_remove_all_for_alert")
        .withParameter("self", variable_requirement_list_instance_address)
        .withParameter("alert_id", alert_id);

    temperature_requirement_list_add(temperature_requirement);
    temperature_requirement_list_for_each((VariableRequirementListForEachCb)for_each_cb);
    temperature_requirement_list_remove_all_for_alert(alert_id);
}
