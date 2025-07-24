#include "CppUTest/TestHarness.h"

extern "C"
{
#include "temperature_value.h"
}

TEST_GROUP(TemperatureValue)
{
};

TEST(TemperatureValue, GetWhatWeSetValue1)
{
   temperature_value temperature_value;
   temperature set_temperature = 1;
   temperature_value_set(&temperature_value, set_temperature);
   temperature retrieved_temperature = temperature_value_get(&temperature_value);
   CHECK_EQUAL(set_temperature, retrieved_temperature);
}

TEST(TemperatureValue, GetWhatWeSetValue22)
{
   temperature_value temperature_value;
   temperature set_temperature = 22;
   temperature_value_set(&temperature_value, set_temperature);
   temperature retrieved_temperature = temperature_value_get(&temperature_value);
   CHECK_EQUAL(set_temperature, retrieved_temperature);
}
