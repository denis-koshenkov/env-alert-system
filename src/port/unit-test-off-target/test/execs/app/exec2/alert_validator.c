#include "CppUTest/TestHarness_c.h"

#include "config.h"
#include "alert_validator.h"

static void populate_valid_alert_condition(MsgTransceiverAlertCondition *const alert_condition)
{
    /* Just one variable requirement for simplicity */
    alert_condition->num_variable_requirements = 1;
    alert_condition->variable_requirements[0].variable_identifier = MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_TEMPERATURE;
    alert_condition->variable_requirements[0].operator = MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_GEQ;
    /* TODO: come up with a valid temperature value here */
    alert_condition->variable_requirements[0].constraint_value.temperature = 0;
    /* Since this is the only variable requirement, it is also the last one in the only ORed requirement */
    alert_condition->variable_requirements[0].is_last_in_ored_requirement = true;
}

static void populate_valid_alert(MsgTransceiverAlert *alert)
{
    alert->alert_id = 0;
    alert->led_color = MSG_TRANSCEIVER_LED_COLOR_RED;
    alert->led_pattern = MSG_TRANSCEIVER_LED_PATTERN_STATIC;
    alert->notification_type.connectivity = 1;
    alert->notification_type.led = 1;
    populate_valid_alert_condition(&(alert->alert_condition));
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

TEST_C(AlertValidator, LedColor100Invalid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_color = 100;
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

TEST_C(AlertValidator, LedPattern55Invalid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.led_pattern = 55;
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
    alert.led_color = 72;
    alert.led_pattern = 44;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, InvalidAlertIdInvalid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    uint8_t invalid_alert_id = CONFIG_ALERT_VALIDATOR_MAX_ALLOWED_ALERT_ID + 1;
    alert.alert_id = invalid_alert_id;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, MaxAllowedAlertIdValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_id = CONFIG_ALERT_VALIDATOR_MAX_ALLOWED_ALERT_ID;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, EmptyAlertConditionInvalid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.num_variable_requirements = 0;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, VariableIdentifier0xFFInvalid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier = 0xFF;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, VariableIdentifier50Invalid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier = 50;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, VariableIdentifierTemperatureValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier =
        MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_TEMPERATURE;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, VariableIdentifierPressureValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier = MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_PRESSURE;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, VariableIdentifierHumidityValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier = MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_HUMIDITY;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, VariableIdentifierLightIntensityValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier =
        MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_LIGHT_INTENSITY;
    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}
