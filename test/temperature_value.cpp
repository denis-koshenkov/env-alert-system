#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "temperature_value.h"

TEST_GROUP(TemperatureValue){};

TEST(TemperatureValue, GetWhatWeSet22)
{
    temperature_value tv = temperature_value_create();
    temperature value_set = 22;
    temperature_value_set(tv, value_set);
    temperature value_get = temperature_value_get(tv);
    CHECK_EQUAL(value_set, value_get);
}
