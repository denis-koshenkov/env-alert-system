#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP_C_WRAPPER(MsgTransceiver)
{
    TEST_GROUP_C_SETUP_WRAPPER(MsgTransceiver);
};

TEST_C_WRAPPER(MsgTransceiver, Alert0Silenced);
TEST_C_WRAPPER(MsgTransceiver, Alert0Raised);
TEST_C_WRAPPER(MsgTransceiver, Alert1Silenced);
TEST_C_WRAPPER(MsgTransceiver, Alert8RaisedFailed);
TEST_C_WRAPPER(MsgTransceiver, MessageSentCbCalledWithUserData);
