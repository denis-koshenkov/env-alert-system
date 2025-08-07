#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "humidity_value.h"

TEST_GROUP(HumidityValue){};

TEST(HumidityValue, createCallsValueHolderCreateWithSize1)
{
    mock().expectOneCall("value_holder_create").withParameter("value_size", 1).andReturnValue((void *)NULL);

    humidity_value hv = humidity_value_create();
}

TEST(HumidityValue, setCallsValueHolderUsingInstanceReturnedByCreate)
{
    void *value_holder_instance_address = (void *)0x333;
    mock().expectOneCall("value_holder_create").ignoreOtherParameters().andReturnValue(value_holder_instance_address);
    mock().expectOneCall("value_holder_set").withParameter("vh", value_holder_instance_address).ignoreOtherParameters();

    humidity_value hv = humidity_value_create();
    humidity humidity = 78;
    humidity_value_set(hv, humidity);
}

TEST(HumidityValue, setPassesPointerToArgumentToValueHolderSet)
{
    /* Pass value size to mock object, so that it can pass it to the `size` parameter of `withMemoryBufferParameter()`*/
    mock().setData("value_holder_value_size", (unsigned int)sizeof(humidity));

    humidity humidity = 1;
    mock().expectOneCall("value_holder_create").withParameter("value_size", 1).andReturnValue((void *)NULL);
    mock()
        .expectOneCall("value_holder_set")
        .withMemoryBufferParameter("value", (const uint8_t *)&humidity, sizeof(humidity))
        .ignoreOtherParameters();

    humidity_value hv = humidity_value_create();
    humidity_value_set(hv, humidity);
}

TEST(HumidityValue, getReturnsValueReceivedFromValueHolderGet)
{
    humidity h = 65;
    mock().expectOneCall("value_holder_create").withParameter("value_size", 1).andReturnValue((void *)NULL);
    mock()
        .expectOneCall("value_holder_get")
        .withOutputParameterReturning("value", &h, sizeof(humidity))
        .ignoreOtherParameters();

    humidity_value hv = humidity_value_create();
    humidity received_humidity = humidity_value_get(hv);
    CHECK_EQUAL(h, received_humidity);
}

TEST(HumidityValue, getCallsValueHolderGetUsingInstanceReturnedByCreate)
{
    void *value_holder_instance_address = (void *)0x5678;
    mock().expectOneCall("value_holder_create").ignoreOtherParameters().andReturnValue(value_holder_instance_address);
    mock().expectOneCall("value_holder_get").withParameter("vh", value_holder_instance_address).ignoreOtherParameters();

    humidity_value hv = humidity_value_create();
    humidity humidity = humidity_value_get(hv);
}

TEST(HumidityValue, isValueChangedReturnsValueReceivedFromValueHolderValueFalse)
{
    bool is_value_changed_from_value_holder = false;
    mock().expectOneCall("value_holder_create").withParameter("value_size", 1).andReturnValue((void *)NULL);
    mock()
        .expectOneCall("value_holder_is_value_changed")
        .ignoreOtherParameters()
        .andReturnValue(is_value_changed_from_value_holder);

    humidity_value hv = humidity_value_create();
    bool is_value_changed_from_humidity_value = humidity_value_is_value_changed(hv);
    CHECK_EQUAL(is_value_changed_from_value_holder, is_value_changed_from_humidity_value);
}

TEST(HumidityValue, isValueChangedReturnsValueReceivedFromValueHolderValueTrue)
{
    bool is_value_changed_from_value_holder = true;
    mock().expectOneCall("value_holder_create").withParameter("value_size", 1).andReturnValue((void *)NULL);
    mock()
        .expectOneCall("value_holder_is_value_changed")
        .ignoreOtherParameters()
        .andReturnValue(is_value_changed_from_value_holder);

    humidity_value hv = humidity_value_create();
    bool is_value_changed_from_humidity_value = humidity_value_is_value_changed(hv);
    CHECK_EQUAL(is_value_changed_from_value_holder, is_value_changed_from_humidity_value);
}

TEST(HumidityValue, isValueChangedCallsValueHolderUsingInstanceReturnedByCreate)
{
    void *value_holder_instance_address = (void *)0x35FF;
    mock().expectOneCall("value_holder_create").ignoreOtherParameters().andReturnValue(value_holder_instance_address);
    mock()
        .expectOneCall("value_holder_is_value_changed")
        .withParameter("vh", value_holder_instance_address)
        .ignoreOtherParameters()
        .andReturnValue(true);

    humidity_value hv = humidity_value_create();
    bool is_value_changed = humidity_value_is_value_changed(hv);
}
