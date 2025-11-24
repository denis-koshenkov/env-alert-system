#include "CppUTest/TestHarness_c.h"

#include "alert_validator.h"

static void populate_valid_alert(MsgTransceiverAlert *alert)
{
    alert->led_color = MSG_TRANSCEIVER_LED_COLOR_RED;
    alert->led_pattern = MSG_TRANSCEIVER_LED_PATTERN_STATIC;
    alert->notification_type.connectivity = 1;
    alert->notification_type.led = 1;
}

TEST_C(AlertValidator, LedColor0xFFInvalid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_color = 0xFF;
    alert.notification_type.led = 1;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, LedColorInvalidInvalid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_color = MSG_TRANSCEIVER_LED_COLOR_INVALID;
    alert.notification_type.led = 1;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, LedColorRedValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_color = MSG_TRANSCEIVER_LED_COLOR_RED;
    alert.notification_type.led = 1;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, LedColorGreenValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_color = MSG_TRANSCEIVER_LED_COLOR_GREEN;
    alert.notification_type.led = 1;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, LedColorBlueValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_color = MSG_TRANSCEIVER_LED_COLOR_BLUE;
    alert.notification_type.led = 1;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, LedPattern0xFFInvalid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_pattern = 0xFF;
    alert.notification_type.led = 1;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, LedPatternInvalidInvalid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_pattern = MSG_TRANSCEIVER_LED_PATTERN_INVALID;
    alert.notification_type.led = 1;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, LedPatternStaticValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_pattern = MSG_TRANSCEIVER_LED_PATTERN_STATIC;
    alert.notification_type.led = 1;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, LedPatternAlertValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_pattern = MSG_TRANSCEIVER_LED_PATTERN_ALERT;
    alert.notification_type.led = 1;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, AllNotificationsDisabledInvalid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.notification_type.connectivity = 0;
    alert.notification_type.led = 0;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, ConnectivityNotificationValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.notification_type.connectivity = 1;
    alert.notification_type.led = 0;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, LedNotificationValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.notification_type.connectivity = 0;
    alert.notification_type.led = 1;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, AllNotificationsEnabledValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.notification_type.connectivity = 1;
    alert.notification_type.led = 1;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, GarbageColorPatternLedNotificationDisabledValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.notification_type.led = 0;
    alert.led_color = MSG_TRANSCEIVER_LED_COLOR_INVALID;
    alert.led_pattern = MSG_TRANSCEIVER_LED_PATTERN_INVALID;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}
