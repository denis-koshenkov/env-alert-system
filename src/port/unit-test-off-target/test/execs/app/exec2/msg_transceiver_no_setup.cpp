#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP_C_WRAPPER(MsgTransceiverNoSetup){};

TEST_C_WRAPPER(MsgTransceiverNoSetup, SendAlertStatusChangeMessageCalledBeforeInit);
