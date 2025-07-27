#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

extern "C"
{
#include "temperature_value.h"
}

TEST_GROUP(TemperatureValue){};

TEST(TemperatureValue, GetWhatWeSetValue1)
{
    temperature_value t = temperature_value_create();
    temperature set_temperature = 1;
    temperature_value_set(t, set_temperature);
    temperature retrieved_temperature = temperature_value_get(t);
    CHECK_EQUAL(set_temperature, retrieved_temperature);
}

TEST(TemperatureValue, GetWhatWeSetValue22)
{
    temperature_value t = temperature_value_create();
    temperature set_temperature = 22;
    temperature_value_set(t, set_temperature);
    temperature retrieved_temperature = temperature_value_get(t);
    CHECK_EQUAL(set_temperature, retrieved_temperature);
}

TEST(TemperatureValue, SetRaisesAssertNullPointer)
{
    temperature set_temperature = 22;
    TestAssertPlugin::expectAssertion();
    temperature_value_set(NULL, set_temperature);
}

TEST(TemperatureValue, GetRaisesAssertNullPointer)
{
    TestAssertPlugin::expectAssertion();
    temperature value = temperature_value_get(NULL);
}
