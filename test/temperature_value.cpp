#include "CppUTest/TestHarness.h"

extern "C"
{
#include "temperature_value.h"
}

TEST_GROUP(TemperatureValue)
{
};

TEST(TemperatureValue, GetWhatWeSet)
{
   temperature set_temperature = 1;
   temperature_value_set(set_temperature);
   temperature retrieved_temperature = temperature_value_get();
   CHECK_EQUAL(set_temperature, retrieved_temperature);
}
