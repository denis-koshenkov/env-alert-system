#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP_C_WRAPPER(AlertValidator){};

TEST_C_WRAPPER(AlertValidator, LedColor0xFFInvalid);
TEST_C_WRAPPER(AlertValidator, LedColorInvalidInvalid);
TEST_C_WRAPPER(AlertValidator, LedColorRedValid);
TEST_C_WRAPPER(AlertValidator, LedColorGreenValid);
TEST_C_WRAPPER(AlertValidator, LedColorBlueValid);
TEST_C_WRAPPER(AlertValidator, LedPattern0xFFInvalid);
TEST_C_WRAPPER(AlertValidator, LedPatternInvalidInvalid);
TEST_C_WRAPPER(AlertValidator, LedPatternStaticValid);
TEST_C_WRAPPER(AlertValidator, LedPatternAlertValid);
TEST_C_WRAPPER(AlertValidator, AllNotificationsDisabledInvalid);
TEST_C_WRAPPER(AlertValidator, ConnectivityNotificationValid);
TEST_C_WRAPPER(AlertValidator, LedNotificationValid);
TEST_C_WRAPPER(AlertValidator, AllNotificationsEnabledValid);
TEST_C_WRAPPER(AlertValidator, GarbageColorPatternLedNotificationDisabledValid);
