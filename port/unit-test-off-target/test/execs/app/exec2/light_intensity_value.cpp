#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "light_intensity_value.h"

TEST_GROUP(LightIntensityValue){};

TEST(LightIntensityValue, createCallsValueHolderCreateWithSize2)
{
    mock().expectOneCall("value_holder_create").withParameter("value_size", 2).andReturnValue((void *)NULL);

    LightIntensityValue liv = light_intensity_value_create();
}

TEST(LightIntensityValue, setCallsValueHolderUsingInstanceReturnedByCreate)
{
    void *value_holder_instance_address = (void *)0xA6;
    mock().expectOneCall("value_holder_create").ignoreOtherParameters().andReturnValue(value_holder_instance_address);
    mock().expectOneCall("value_holder_set").withParameter("vh", value_holder_instance_address).ignoreOtherParameters();

    LightIntensityValue liv = light_intensity_value_create();
    LightIntensity light_intensity = 222;
    light_intensity_value_set(liv, light_intensity);
}

TEST(LightIntensityValue, setPassesPointerToArgumentToValueHolderSet)
{
    /* Pass value size to mock object, so that it can pass it to the `size` parameter of `withMemoryBufferParameter()`*/
    mock().setData("value_holder_value_size", (unsigned int)sizeof(LightIntensity));

    LightIntensity light_intensity = 11122;
    mock().expectOneCall("value_holder_create").withParameter("value_size", 2).andReturnValue((void *)NULL);
    mock()
        .expectOneCall("value_holder_set")
        .withMemoryBufferParameter("value", (const uint8_t *)&light_intensity, sizeof(LightIntensity))
        .ignoreOtherParameters();

    LightIntensityValue liv = light_intensity_value_create();
    light_intensity_value_set(liv, light_intensity);
}

TEST(LightIntensityValue, getReturnsValueReceivedFromValueHolderGet)
{
    LightIntensity l = 65;
    mock().expectOneCall("value_holder_create").withParameter("value_size", 2).andReturnValue((void *)NULL);
    mock()
        .expectOneCall("value_holder_get")
        .withOutputParameterReturning("value", &l, sizeof(LightIntensity))
        .ignoreOtherParameters();

    LightIntensityValue liv = light_intensity_value_create();
    LightIntensity received_light_intensity = light_intensity_value_get(liv);
    CHECK_EQUAL(l, received_light_intensity);
}

TEST(LightIntensityValue, getCallsValueHolderGetUsingInstanceReturnedByCreate)
{
    void *value_holder_instance_address = (void *)0xA55A;
    mock().expectOneCall("value_holder_create").ignoreOtherParameters().andReturnValue(value_holder_instance_address);
    mock().expectOneCall("value_holder_get").withParameter("vh", value_holder_instance_address).ignoreOtherParameters();

    LightIntensityValue liv = light_intensity_value_create();
    LightIntensity light_intensity = light_intensity_value_get(liv);
}

TEST(LightIntensityValue, isValueChangedReturnsValueReceivedFromValueHolderValueFalse)
{
    bool is_value_changed_from_value_holder = false;
    mock().expectOneCall("value_holder_create").withParameter("value_size", 2).andReturnValue((void *)NULL);
    mock()
        .expectOneCall("value_holder_is_value_changed")
        .ignoreOtherParameters()
        .andReturnValue(is_value_changed_from_value_holder);

    LightIntensityValue liv = light_intensity_value_create();
    bool is_value_changed_from_light_intensity_value = light_intensity_value_is_value_changed(liv);
    CHECK_EQUAL(is_value_changed_from_value_holder, is_value_changed_from_light_intensity_value);
}

TEST(LightIntensityValue, isValueChangedReturnsValueReceivedFromValueHolderValueTrue)
{
    bool is_value_changed_from_value_holder = true;
    mock().expectOneCall("value_holder_create").withParameter("value_size", 2).andReturnValue((void *)NULL);
    mock()
        .expectOneCall("value_holder_is_value_changed")
        .ignoreOtherParameters()
        .andReturnValue(is_value_changed_from_value_holder);

    LightIntensityValue liv = light_intensity_value_create();
    bool is_value_changed_from_light_intensity_value = light_intensity_value_is_value_changed(liv);
    CHECK_EQUAL(is_value_changed_from_value_holder, is_value_changed_from_light_intensity_value);
}

TEST(LightIntensityValue, isValueChangedCallsValueHolderUsingInstanceReturnedByCreate)
{
    void *value_holder_instance_address = (void *)0x1111;
    mock().expectOneCall("value_holder_create").ignoreOtherParameters().andReturnValue(value_holder_instance_address);
    mock()
        .expectOneCall("value_holder_is_value_changed")
        .withParameter("vh", value_holder_instance_address)
        .ignoreOtherParameters()
        .andReturnValue(true);

    LightIntensityValue liv = light_intensity_value_create();
    bool is_value_changed = light_intensity_value_is_value_changed(liv);
}
