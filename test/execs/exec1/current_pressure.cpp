#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "current_pressure.h"

TEST_GROUP(CurrentPressure){};

/* Tests that pressure_value_create() is called only once. Also tests that all wrapper functions - get, set,
 * is_changed - use the instance returned by pressure_value_create() and correctly propagate function calls to its
 * pressure_value counterparts.
 *
 * It is all in one test because the order of execution of different tests is not guaranteed, so we would have no way of
 * knowing which test would actually call pressure_value_create(), since it only gets called when a
 * current_pressure function gets called for the first time. */
TEST(CurrentPressure, CreateIsCalledOnlyOnceAndGetSetValueChangedWrappersPropagateCalls)
{
    void *pressure_value_instance_address = (void *)0x625A;
    Pressure set_1 = 20;
    Pressure get_1_expected_value = 25;
    bool is_changed_1_expected_value = true;
    bool is_changed_2_expected_value = false;

    mock().expectOneCall("pressure_value_create").andReturnValue(pressure_value_instance_address);
    mock()
        .expectOneCall("pressure_value_set")
        .withParameter("pv", pressure_value_instance_address)
        .withParameter("pressure", set_1);
    mock().expectOneCall("pressure_value_get").withParameter("pv", pressure_value_instance_address).andReturnValue(25);
    mock()
        .expectOneCall("pressure_value_is_value_changed")
        .withParameter("pv", pressure_value_instance_address)
        .andReturnValue(is_changed_1_expected_value);
    mock()
        .expectOneCall("pressure_value_is_value_changed")
        .withParameter("pv", pressure_value_instance_address)
        .andReturnValue(is_changed_2_expected_value);

    current_pressure_set(set_1);

    Pressure get_1_actual_value = current_pressure_get();
    CHECK_EQUAL(get_1_actual_value, get_1_expected_value);

    bool is_changed_1 = current_pressure_is_changed();
    CHECK_EQUAL(is_changed_1, is_changed_1_expected_value);

    bool is_changed_2 = current_pressure_is_changed();
    CHECK_EQUAL(is_changed_2, is_changed_2_expected_value);
}
