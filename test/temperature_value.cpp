#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

extern "C"
{
#include "temperature_value.h"
}

TEST_GROUP(TemperatureValue){};

TEST(TemperatureValue, GetWhatWeSetValue1)
{
    temperature_value temperature_value;
    temperature_value_init(&temperature_value);
    temperature set_temperature = 1;
    temperature_value_set(&temperature_value, set_temperature);
    temperature retrieved_temperature = temperature_value_get(&temperature_value);
    CHECK_EQUAL(set_temperature, retrieved_temperature);
}

TEST(TemperatureValue, GetWhatWeSetValue22)
{
    temperature_value temperature_value;
    temperature_value_init(&temperature_value);
    temperature set_temperature = 22;
    temperature_value_set(&temperature_value, set_temperature);
    temperature retrieved_temperature = temperature_value_get(&temperature_value);
    CHECK_EQUAL(set_temperature, retrieved_temperature);
}

TEST(TemperatureValue, IsValueChangedReturnsTrueAfterSetIsCalled)
{
    temperature_value temperature_value;
    temperature_value_init(&temperature_value);
    temperature_value_set(&temperature_value, 23);
    bool is_value_changed = temperature_value_is_value_changed(&temperature_value);
    CHECK(is_value_changed);
}

TEST(TemperatureValue, IsValueChangedReturnsFalseAfterSetIsCalledTwiceWithSameValue)
{
    temperature_value temperature_value;
    temperature_value_init(&temperature_value);
    temperature value = 11;
    temperature_value_set(&temperature_value, value);
    temperature_value_set(&temperature_value, value);
    bool is_value_changed = temperature_value_is_value_changed(&temperature_value);
    CHECK(!is_value_changed);
}

TEST(TemperatureValue, IsValueChangedReturnsTrueAfterSetIsCalledTwiceWithDifferentValues)
{
    temperature_value temperature_value;
    temperature_value_init(&temperature_value);
    temperature_value_set(&temperature_value, 10);
    temperature_value_set(&temperature_value, 20);
    bool is_value_changed = temperature_value_is_value_changed(&temperature_value);
    CHECK(is_value_changed);
}

TEST(TemperatureValue, IsValueChangedReturnsTrueAfterSetIsCalledManyTimesAndFinallyTwiceWithDifferentValues)
{
    temperature_value temperature_value;
    temperature_value_init(&temperature_value);
    temperature_value_set(&temperature_value, 20);
    temperature_value_set(&temperature_value, 30);
    temperature_value_set(&temperature_value, 30);
    temperature_value_set(&temperature_value, 30);
    temperature_value_set(&temperature_value, 100);
    bool is_value_changed = temperature_value_is_value_changed(&temperature_value);
    CHECK(is_value_changed);
}

TEST(TemperatureValue, IsValueChangedReturnsFalseAfterSetIsCalledManyTimesAndFinallyTwiceWithSameValues)
{
    temperature_value temperature_value;
    temperature_value_init(&temperature_value);
    temperature_value_set(&temperature_value, 50);
    temperature_value_set(&temperature_value, 40);
    temperature_value_set(&temperature_value, 30);
    temperature_value_set(&temperature_value, 20);
    temperature_value_set(&temperature_value, 20);
    bool is_value_changed = temperature_value_is_value_changed(&temperature_value);
    CHECK(!is_value_changed);
}

TEST(TemperatureValue, AssertTest)
{
    temperature_value temperature_value;
    temperature_value_init(&temperature_value);
    TestAssertPlugin::expectAssertion();
    temperature_value_assert_test(NULL);
}
