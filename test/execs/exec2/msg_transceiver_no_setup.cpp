#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP_C_WRAPPER(MsgTransceiverNoSetup)
{
    TEST_GROUP_C_SETUP_WRAPPER(MsgTransceiverNoSetup);
    TEST_GROUP_C_TEARDOWN_WRAPPER(MsgTransceiverNoSetup);
};

TEST_C_WRAPPER(MsgTransceiverNoSetup, SendAlertStatusChangeMessageCalledBeforeInit);
TEST_C_WRAPPER(MsgTransceiverNoSetup, SetAddAlertCbCalledBeforeInit);
TEST_C_WRAPPER(MsgTransceiverNoSetup, SetRemoveAlertCbCalledBeforeInit);
TEST_C_WRAPPER(MsgTransceiverNoSetup, InitCalledTwice);
TEST_C_WRAPPER(MsgTransceiverNoSetup, DeinitDoesNothingIfAlreadyDeinitialized);
