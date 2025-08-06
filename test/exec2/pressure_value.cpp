#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "pressure_value.h"

TEST_GROUP(PressureValue){};

TEST(PressureValue, createCallsValueHolderCreateWithSize2)
{
    mock().expectOneCall("value_holder_create").withParameter("value_size", 2).andReturnValue((void *)NULL);

    pressure_value pv = pressure_value_create();
}

TEST(PressureValue, setCallsValueHolderUsingInstanceReturnedByCreate)
{
    void *value_holder_instance_address = (void *)0xA6;
    mock().expectOneCall("value_holder_create").ignoreOtherParameters().andReturnValue(value_holder_instance_address);
    mock().expectOneCall("value_holder_set").withParameter("vh", value_holder_instance_address).ignoreOtherParameters();

    pressure_value pv = pressure_value_create();
    pressure p = 500;
    pressure_value_set(pv, p);
}

TEST(PressureValue, setPassesPointerToArgumentToValueHolderSet)
{
    /* Pass value size to mock object, so that it can pass it to the `size` parameter of `withMemoryBufferParameter()`*/
    mock().setData("value_holder_value_size", (unsigned int)sizeof(pressure));

    pressure p = 10000;
    mock().expectOneCall("value_holder_create").withParameter("value_size", 2).andReturnValue((void *)NULL);
    mock()
        .expectOneCall("value_holder_set")
        .withMemoryBufferParameter("value", (const uint8_t *)&p, sizeof(pressure))
        .ignoreOtherParameters();

    pressure_value pv = pressure_value_create();
    pressure_value_set(pv, p);
}

TEST(PressureValue, getReturnsValueReceivedFromValueHolderGet)
{
    pressure p = 65;
    mock().expectOneCall("value_holder_create").withParameter("value_size", 2).andReturnValue((void *)NULL);
    mock()
        .expectOneCall("value_holder_get")
        .withOutputParameterReturning("value", &p, sizeof(pressure))
        .ignoreOtherParameters();

    pressure_value pv = pressure_value_create();
    pressure received_pressure = pressure_value_get(pv);
    CHECK_EQUAL(p, received_pressure);
}

TEST(PressureValue, getCallsValueHolderGetUsingInstanceReturnedByCreate)
{
    void *value_holder_instance_address = (void *)0xA55A;
    mock().expectOneCall("value_holder_create").ignoreOtherParameters().andReturnValue(value_holder_instance_address);
    mock().expectOneCall("value_holder_get").withParameter("vh", value_holder_instance_address).ignoreOtherParameters();

    pressure_value pv = pressure_value_create();
    pressure pressure = pressure_value_get(pv);
}

TEST(PressureValue, isValueChangedReturnsValueReceivedFromValueHolderValueFalse)
{
    bool is_value_changed_from_value_holder = false;
    mock().expectOneCall("value_holder_create").withParameter("value_size", 2).andReturnValue((void *)NULL);
    mock()
        .expectOneCall("value_holder_is_value_changed")
        .ignoreOtherParameters()
        .andReturnValue(is_value_changed_from_value_holder);

    pressure_value pv = pressure_value_create();
    bool is_value_changed_from_pressure_value = pressure_value_is_value_changed(pv);
    CHECK_EQUAL(is_value_changed_from_value_holder, is_value_changed_from_pressure_value);
}

TEST(PressureValue, isValueChangedReturnsValueReceivedFromValueHolderValueTrue)
{
    bool is_value_changed_from_value_holder = true;
    mock().expectOneCall("value_holder_create").withParameter("value_size", 2).andReturnValue((void *)NULL);
    mock()
        .expectOneCall("value_holder_is_value_changed")
        .ignoreOtherParameters()
        .andReturnValue(is_value_changed_from_value_holder);

    pressure_value pv = pressure_value_create();
    bool is_value_changed_from_pressure_value = pressure_value_is_value_changed(pv);
    CHECK_EQUAL(is_value_changed_from_value_holder, is_value_changed_from_pressure_value);
}

TEST(PressureValue, isValueChangedCallsValueHolderUsingInstanceReturnedByCreate)
{
    void *value_holder_instance_address = (void *)0x1111;
    mock().expectOneCall("value_holder_create").ignoreOtherParameters().andReturnValue(value_holder_instance_address);
    mock()
        .expectOneCall("value_holder_is_value_changed")
        .withParameter("vh", value_holder_instance_address)
        .ignoreOtherParameters()
        .andReturnValue(true);

    pressure_value pv = pressure_value_create();
    bool is_value_changed = pressure_value_is_value_changed(pv);
}
