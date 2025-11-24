#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP_C_WRAPPER(AlertValidator){};

TEST_C_WRAPPER(AlertValidator, LedColor0xFFInvalid);
TEST_C_WRAPPER(AlertValidator, LedColor100Invalid);
TEST_C_WRAPPER(AlertValidator, LedColorRedValid);
TEST_C_WRAPPER(AlertValidator, LedColorGreenValid);
TEST_C_WRAPPER(AlertValidator, LedColorBlueValid);
TEST_C_WRAPPER(AlertValidator, LedPattern0xFFInvalid);
TEST_C_WRAPPER(AlertValidator, LedPattern55Invalid);
TEST_C_WRAPPER(AlertValidator, LedPatternStaticValid);
TEST_C_WRAPPER(AlertValidator, LedPatternAlertValid);
TEST_C_WRAPPER(AlertValidator, AllNotificationsDisabledInvalid);
TEST_C_WRAPPER(AlertValidator, ConnectivityNotificationValid);
TEST_C_WRAPPER(AlertValidator, LedNotificationValid);
TEST_C_WRAPPER(AlertValidator, AllNotificationsEnabledValid);
TEST_C_WRAPPER(AlertValidator, GarbageColorPatternLedNotificationDisabledValid);
TEST_C_WRAPPER(AlertValidator, InvalidAlertIdInvalid);
TEST_C_WRAPPER(AlertValidator, MaxAllowedAlertIdValid);
TEST_C_WRAPPER(AlertValidator, EmptyAlertConditionInvalid);
TEST_C_WRAPPER(AlertValidator, VariableIdentifier0xFFInvalid);
TEST_C_WRAPPER(AlertValidator, VariableIdentifier50Invalid);
TEST_C_WRAPPER(AlertValidator, VariableIdentifierTemperatureValid);
TEST_C_WRAPPER(AlertValidator, VariableIdentifierPressureValid);
TEST_C_WRAPPER(AlertValidator, VariableIdentifierHumidityValid);
TEST_C_WRAPPER(AlertValidator, VariableIdentifierLightIntensityValid);
