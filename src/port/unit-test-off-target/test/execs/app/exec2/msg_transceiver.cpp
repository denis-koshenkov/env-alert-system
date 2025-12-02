#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP_C_WRAPPER(MsgTransceiver)
{
    TEST_GROUP_C_SETUP_WRAPPER(MsgTransceiver);
    TEST_GROUP_C_TEARDOWN_WRAPPER(MsgTransceiver);
};

TEST_C_WRAPPER(MsgTransceiver, Alert0Silenced);
TEST_C_WRAPPER(MsgTransceiver, Alert0Raised);
TEST_C_WRAPPER(MsgTransceiver, Alert1Silenced);
TEST_C_WRAPPER(MsgTransceiver, Alert8RaisedFailed);
TEST_C_WRAPPER(MsgTransceiver, MessageSentCbCalledWithUserData);
TEST_C_WRAPPER(MsgTransceiver, RemoveAlert0);
TEST_C_WRAPPER(MsgTransceiver, RemoveAlert1);
TEST_C_WRAPPER(MsgTransceiver, ReceiveCbCalledWithNumBytes0);
TEST_C_WRAPPER(MsgTransceiver, RemoveAlertMessageWithNoAlertId);
TEST_C_WRAPPER(MsgTransceiver, RemoveAlertMessageTooManyBytes);
TEST_C_WRAPPER(MsgTransceiver, ReceiveCbBytesNullPointerAssert);
TEST_C_WRAPPER(MsgTransceiver, RemoveAlertCbExecutedWithUserData);
TEST_C_WRAPPER(MsgTransceiver, AddAlert0);
TEST_C_WRAPPER(MsgTransceiver, AddAlert1);
