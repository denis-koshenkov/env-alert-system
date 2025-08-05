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
    mock()
        .expectOneCall("value_holder_create")
        .withParameter("value_size", 2)
        .andReturnValue(value_holder_instance_address);
    temperature_value tv = temperature_value_create();
    temperature t = 22;
    mock().expectOneCall("value_holder_set").withParameter("vh", value_holder_instance_address);
    temperature_value_set(tv, t);
}
