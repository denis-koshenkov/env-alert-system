#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP_C_WRAPPER(TemperatureRequirement){};

TEST_C_WRAPPER(TemperatureRequirement, evaluateReturnsTrueOperatorGEQValueGreater);
