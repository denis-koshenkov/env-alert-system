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
TEST_C_WRAPPER(MsgTransceiver, AddAlert2);
TEST_C_WRAPPER(MsgTransceiver, AddAlert3);
TEST_C_WRAPPER(MsgTransceiver, InvalidMessageId);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessageOnlyMessageId);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage2ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage3ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage4ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage5ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage6ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage7ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage8ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage9ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage10ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage11ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage12ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage13ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage14ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage15ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage16ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage17ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage18ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage19ValidBytesValidAlert);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage19ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage20ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage21ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage22ValidBytes);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage17ValidBytesHumidity);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage18ValidBytesHumidity);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage17ValidBytesLightIntensity);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage18ValidBytesLightIntensity);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage19ValidBytesLightIntensity);
TEST_C_WRAPPER(MsgTransceiver, AddAlertMessage20ValidBytesLightIntensity);
TEST_C_WRAPPER(MsgTransceiver, AddAlertInvalidNotificationType);
