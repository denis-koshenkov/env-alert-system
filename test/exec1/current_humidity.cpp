#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "current_humidity.h"

TEST_GROUP(CurrentHumidity){};

/* Tests that humidity_value_create() is called only once. Also tests that all wrapper functions - get, set,
 * is_changed - use the instance returned by humidity_value_create() and correctly propagate function calls to its
 * humidity_value counterparts.
 *
 * It is all in one test because the order of execution of different tests is not guaranteed, so we would have no way of
 * knowing which test would actually call humidity_value_create(), since it only gets called when a
 * current_humidity function gets called for the first time. */
TEST(CurrentHumidity, CreateIsCalledOnlyOnceAndGetSetValueChangedWrappersPropagateCalls)
{
    void *humidity_value_instance_address = (void *)0x5A01;
    Humidity set_1 = 200;
    Humidity get_1_expected_value = 250;
    bool is_changed_1_expected_value = true;
    bool is_changed_2_expected_value = false;

    mock().expectOneCall("humidity_value_create").andReturnValue(humidity_value_instance_address);
    mock()
        .expectOneCall("humidity_value_set")
        .withParameter("hv", humidity_value_instance_address)
        .withParameter("humidity", set_1);
    mock()
        .expectOneCall("humidity_value_get")
        .withParameter("hv", humidity_value_instance_address)
        .andReturnValue(get_1_expected_value);
    mock()
        .expectOneCall("humidity_value_is_value_changed")
        .withParameter("hv", humidity_value_instance_address)
        .andReturnValue(is_changed_1_expected_value);
    mock()
        .expectOneCall("humidity_value_is_value_changed")
        .withParameter("hv", humidity_value_instance_address)
        .andReturnValue(is_changed_2_expected_value);

    current_humidity_set(set_1);

    Humidity get_1_actual_value = current_humidity_get();
    CHECK_EQUAL(get_1_expected_value, get_1_actual_value);

    bool is_changed_1 = current_humidity_is_changed();
    CHECK_EQUAL(is_changed_1_expected_value, is_changed_1);

    bool is_changed_2 = current_humidity_is_changed();
    CHECK_EQUAL(is_changed_2_expected_value, is_changed_2);
}
