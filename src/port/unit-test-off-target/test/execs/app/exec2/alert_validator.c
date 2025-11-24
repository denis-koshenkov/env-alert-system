#include "CppUTest/TestHarness_c.h"

#include "alert_validator.h"

// MsgTransceiverAlert alert = {
//     .alert_id = 0,
//     .warmup_period = 0,
//     .cooldown_period = 0,
//     .led_color = MSG_TRANSCEIVER_LED_COLOR_RED,
//     .led_pattern = MSG_TRANSCEIVER_LED_PATTERN_STATIC,
// };
// alert.notification_type.connectivity = 1;
// alert.notification_type.led = 1;
// alert.alert_condition.num_variable_requirements = 1;

static void populate_valid_alert(MsgTransceiverAlert *alert)
{
    alert->led_color = MSG_TRANSCEIVER_LED_COLOR_RED;
    alert->led_pattern = MSG_TRANSCEIVER_LED_PATTERN_STATIC;
}

TEST_C(AlertValidator, LedColor0xFFInvalid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_color = 0xFF;
    bool is_valid_alert = alert_validator_is_valid_alert(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, LedColorInvalidInvalid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_color = MSG_TRANSCEIVER_LED_COLOR_INVALID;
    bool is_valid_alert = alert_validator_is_valid_alert(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, LedColorRedValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_color = MSG_TRANSCEIVER_LED_COLOR_RED;
    bool is_valid_alert = alert_validator_is_valid_alert(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, LedColorGreenValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_color = MSG_TRANSCEIVER_LED_COLOR_GREEN;
    bool is_valid_alert = alert_validator_is_valid_alert(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, LedColorBlueValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_color = MSG_TRANSCEIVER_LED_COLOR_BLUE;
    bool is_valid_alert = alert_validator_is_valid_alert(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, LedPattern0xFFInvalid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_pattern = 0xFF;
    bool is_valid_alert = alert_validator_is_valid_alert(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, LedPatternInvalidInvalid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_pattern = MSG_TRANSCEIVER_LED_PATTERN_INVALID;
    bool is_valid_alert = alert_validator_is_valid_alert(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, LedPatternStaticValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_pattern = MSG_TRANSCEIVER_LED_PATTERN_STATIC;
    bool is_valid_alert = alert_validator_is_valid_alert(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, LedPatternAlertValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_pattern = MSG_TRANSCEIVER_LED_PATTERN_ALERT;
    bool is_valid_alert = alert_validator_is_valid_alert(&alert);
    CHECK_C(is_valid_alert);
}
