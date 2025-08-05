#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "temperature_value.h"

TEST_GROUP(TemperatureValue){};

TEST(TemperatureValue, createCallsValueHolderCreateWithSize2)
{
    mock().expectOneCall("value_holder_create").withParameter("value_size", 2).andReturnValue((void *)NULL);

    temperature_value tv = temperature_value_create();
}

TEST(TemperatureValue, setCallsValueHolderUsingInstanceReturnedByCreate)
{
    void *value_holder_instance_address = (void *)0x5A;
    mock().expectOneCall("value_holder_create").ignoreOtherParameters().andReturnValue(value_holder_instance_address);
    mock().expectOneCall("value_holder_set").withParameter("vh", value_holder_instance_address).ignoreOtherParameters();

    temperature_value tv = temperature_value_create();
    temperature t = 22;
    temperature_value_set(tv, t);
}

TEST(TemperatureValue, setPassesPointerToArgumentToValueHolderSet)
{
    /* Pass value size to mock object, so that it can pass it to the `size` parameter of `withMemoryBufferParameter()`*/
    mock().setData("value_holder_value_size", (unsigned int)sizeof(temperature));

    temperature t = 33;
    mock().expectOneCall("value_holder_create").withParameter("value_size", 2).andReturnValue((void *)NULL);
    mock()
        .expectOneCall("value_holder_set")
        .withMemoryBufferParameter("value", (const uint8_t *)&t, sizeof(temperature))
        .ignoreOtherParameters();

    temperature_value tv = temperature_value_create();
    temperature_value_set(tv, t);
}

TEST(TemperatureValue, getReturnsValueReceivedFromValueHolderGet)
{
    temperature t = 65;
    mock().expectOneCall("value_holder_create").withParameter("value_size", 2).andReturnValue((void *)NULL);
    mock()
        .expectOneCall("value_holder_get")
        .withOutputParameterReturning("value", &t, sizeof(temperature))
        .ignoreOtherParameters();

    temperature_value tv = temperature_value_create();
    temperature received_temperature = temperature_value_get(tv);
    CHECK_EQUAL(t, received_temperature);
}

TEST(TemperatureValue, getCallsValueHolderGetUsingInstanceReturnedByCreate)
{
    void *value_holder_instance_address = (void *)0xFF5A;
    mock().expectOneCall("value_holder_create").ignoreOtherParameters().andReturnValue(value_holder_instance_address);
    mock().expectOneCall("value_holder_get").withParameter("vh", value_holder_instance_address).ignoreOtherParameters();

    temperature_value tv = temperature_value_create();
    temperature temperature = temperature_value_get(tv);
}

TEST(TemperatureValue, isValueChangedReturnsValueReceivedFromValueHolderValueFalse)
{
    bool is_value_changed_from_value_holder = false;
    mock().expectOneCall("value_holder_create").withParameter("value_size", 2).andReturnValue((void *)NULL);
    mock()
        .expectOneCall("value_holder_is_value_changed")
        .ignoreOtherParameters()
        .andReturnValue(is_value_changed_from_value_holder);

    temperature_value tv = temperature_value_create();
    bool is_value_changed_from_temperature_value = temperature_value_is_value_changed(tv);
    CHECK_EQUAL(is_value_changed_from_value_holder, is_value_changed_from_temperature_value);
}

TEST(TemperatureValue, isValueChangedReturnsValueReceivedFromValueHolderValueTrue)
{
    bool is_value_changed_from_value_holder = true;
    mock().expectOneCall("value_holder_create").withParameter("value_size", 2).andReturnValue((void *)NULL);
    mock()
        .expectOneCall("value_holder_is_value_changed")
        .ignoreOtherParameters()
        .andReturnValue(is_value_changed_from_value_holder);

    temperature_value tv = temperature_value_create();
    bool is_value_changed_from_temperature_value = temperature_value_is_value_changed(tv);
    CHECK_EQUAL(is_value_changed_from_value_holder, is_value_changed_from_temperature_value);
}

TEST(TemperatureValue, isValueChangedCallsValueHolderUsingInstanceReturnedByCreate)
{
    void *value_holder_instance_address = (void *)0x11;
    mock().expectOneCall("value_holder_create").ignoreOtherParameters().andReturnValue(value_holder_instance_address);
    mock()
        .expectOneCall("value_holder_is_value_changed")
        .withParameter("vh", value_holder_instance_address)
        .ignoreOtherParameters()
        .andReturnValue(true);

    temperature_value tv = temperature_value_create();
    bool is_value_changed = temperature_value_is_value_changed(tv);
}
