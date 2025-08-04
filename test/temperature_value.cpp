#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "temperature_value.h"

TEST_GROUP(TemperatureValue){};

TEST(TemperatureValue, createCallsValueHolderCreateWithSize2)
{
    mock().expectOneCall("value_holder_create").withParameter("p2", 2);
    temperature_value tv = temperature_value_create();
}
