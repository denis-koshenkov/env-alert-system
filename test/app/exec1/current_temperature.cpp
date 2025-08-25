#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "current_temperature.h"

TEST_GROUP(CurrentTemperature){};

/* Tests that temperature_value_create() is called only once. Also tests that all wrapper functions - get, set,
 * is_changed - use the instance returned by temperature_value_create() and correctly propagate function calls to its
 * temperature_value counterparts.
 *
 * It is all in one test because the order of execution of different tests is not guaranteed, so we would have no way of
 * knowing which test would actually call temperature_value_create(), since it only gets called when a
 * current_temperature function gets called for the first time. */
TEST(CurrentTemperature, CreateIsCalledOnlyOnceAndGetSetValueChangedWrappersPropagateCalls)
{
    void *temperature_value_instance_address = (void *)0x3E;
    Temperature set_1 = 20;
    Temperature get_1_expected_value = 25;
    bool is_changed_1_expected_value = true;
    bool is_changed_2_expected_value = false;

    mock().expectOneCall("temperature_value_create").andReturnValue(temperature_value_instance_address);
    mock()
        .expectOneCall("temperature_value_set")
        .withParameter("tv", temperature_value_instance_address)
        .withParameter("temperature", set_1);
    mock()
        .expectOneCall("temperature_value_get")
        .withParameter("tv", temperature_value_instance_address)
        .andReturnValue(25);
    mock()
        .expectOneCall("temperature_value_is_value_changed")
        .withParameter("tv", temperature_value_instance_address)
        .andReturnValue(is_changed_1_expected_value);
    mock()
        .expectOneCall("temperature_value_is_value_changed")
        .withParameter("tv", temperature_value_instance_address)
        .andReturnValue(is_changed_2_expected_value);

    current_temperature_set(set_1);

    Temperature t = current_temperature_get();
    CHECK_EQUAL(t, get_1_expected_value);

    bool is_changed_1 = current_temperature_is_changed();
    CHECK_EQUAL(is_changed_1, is_changed_1_expected_value);

    bool is_changed_2 = current_temperature_is_changed();
    CHECK_EQUAL(is_changed_2, is_changed_2_expected_value);
}
