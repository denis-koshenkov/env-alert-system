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

TEST(TemperatureValue, IsValueChangedRaisesAssertIfCalledBeforeSet)
{
    temperature_value t = temperature_value_create();
    TestAssertPlugin::expectAssertion();
    temperature value = temperature_value_is_value_changed(t);
}

TEST(TemperatureValue, IsValueChangedReturnsTrueAfterSetIsCalledOnce)
{
    temperature_value t = temperature_value_create();
    temperature_value_set(t, 30);
    bool is_value_changed = temperature_value_is_value_changed(t);
    CHECK(is_value_changed);
}

TEST(TemperatureValue, IsValueChangedReturnsFalseAfterTwoDifferentValuesAreSet)
{
    temperature_value t = temperature_value_create();
    temperature_value_set(t, 30);
    temperature_value_set(t, 20);
    bool is_value_changed = temperature_value_is_value_changed(t);
    CHECK(!is_value_changed);
}
