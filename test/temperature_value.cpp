#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport_c.h"

extern "C"
{
#include "temperature_value.h"
}

TEST_GROUP(TemperatureValue)
{
   void teardown()
   {
      mock_c()->clear();
   }
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

TEST(TemperatureValue, IsValueChangedAssertsIfCalledBeforeSet)
{
   /* Setup */
   mock_c()->expectOneCall("on_assert");
   temperature_value temperature_value;
   /* Exercise */
   bool is_value_changed = temperature_value_is_value_changed(&temperature_value);
   /* Check */
   mock_c()->checkExpectations();
}
