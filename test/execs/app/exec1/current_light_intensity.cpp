#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "current_light_intensity.h"

TEST_GROUP(CurrentLightIntensity){};

/* Tests that light_intensity_value_create() is called only once. Also tests that all wrapper functions - get, set,
 * is_changed - use the instance returned by light_intensity_value_create() and correctly propagate function calls to
 * its light_intensity_value counterparts.
 *
 * It is all in one test because the order of execution of different tests is not guaranteed, so we would have no way of
 * knowing which test would actually call light_intensity_value_create(), since it only gets called when a
 * current_light_intensity function gets called for the first time. */
TEST(CurrentLightIntensity, CreateIsCalledOnlyOnceAndGetSetValueChangedWrappersPropagateCalls)
{
    void *light_intensity_value_instance_address = (void *)0x625A;
    LightIntensity set_1 = 20;
    LightIntensity get_1_expected_value = 25;
    bool is_changed_1_expected_value = true;
    bool is_changed_2_expected_value = false;

    mock().expectOneCall("light_intensity_value_create").andReturnValue(light_intensity_value_instance_address);
    mock()
        .expectOneCall("light_intensity_value_set")
        .withParameter("liv", light_intensity_value_instance_address)
        .withParameter("light_intensity", set_1);
    mock()
        .expectOneCall("light_intensity_value_get")
        .withParameter("liv", light_intensity_value_instance_address)
        .andReturnValue(get_1_expected_value);
    mock()
        .expectOneCall("light_intensity_value_is_value_changed")
        .withParameter("liv", light_intensity_value_instance_address)
        .andReturnValue(is_changed_1_expected_value);
    mock()
        .expectOneCall("light_intensity_value_is_value_changed")
        .withParameter("liv", light_intensity_value_instance_address)
        .andReturnValue(is_changed_2_expected_value);

    current_light_intensity_set(set_1);

    LightIntensity get_1_actual_value = current_light_intensity_get();
    CHECK_EQUAL(get_1_expected_value, get_1_actual_value);

    bool is_changed_1 = current_light_intensity_is_changed();
    CHECK_EQUAL(is_changed_1_expected_value, is_changed_1);

    bool is_changed_2 = current_light_intensity_is_changed();
    CHECK_EQUAL(is_changed_2_expected_value, is_changed_2);
}
