#include "CppUTest/TestHarness_c.h"

#include "config.h"
#include "alert_validator.h"

/**
 * @brief Populate @p requirement with a valid variable requirement.
 *
 * is_last_in_ored_requirement is set to true, because it is possible that every variable requirement in an alert
 * condition is a last one in an ORed requirement. It is not possible, however, that every variable requirement is NOT
 * the last one in an ORed requirement - the very last variable requirement in an alert condition must always have this
 * field set to true.
 *
 * @param requirement Variable requirement to populate.
 */
static void populate_valid_variable_requirement(MsgTransceiverVariableRequirement *const requirement)
{
    requirement->variable_identifier = MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_TEMPERATURE;
    requirement->operator = MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_GEQ;
    requirement->constraint_value.temperature = 200; // 20.0 degrees Celsius
    requirement->is_last_in_ored_requirement = true;
}

/**
 * @brief Add a valid variable requirement to the alert condition.
 *
 * Adds a valid variable requirement at index alert_condition->num_variable_requirements and increments the
 * alert_condition->num_variable_requirements variable.
 *
 * @param alert_condition Alert condition to add the requirement to.
 */
static void add_valid_variable_requirement(MsgTransceiverAlertCondition *const alert_condition)
{
    populate_valid_variable_requirement(
        &(alert_condition->variable_requirements[alert_condition->num_variable_requirements]));
    alert_condition->num_variable_requirements++;
}

/**
 * @brief Populate @p alert_condition with a valid alert condition that contains one variable requirement.
 *
 * @param alert_condition Alert condition to populate.
 */
static void populate_valid_alert_condition(MsgTransceiverAlertCondition *const alert_condition)
{
    /* Add just one variable requirement for simplicity. Tests that need more variable requirements can call
     * add_valid_variable_requirement themselves. */
    alert_condition->num_variable_requirements = 0;
    add_valid_variable_requirement(alert_condition);
}

/**
 * @brief Populates @p alert with a valid alert that contains one variable requirement.
 *
 * @param alert Alert to populate.
 */
static void populate_valid_alert(MsgTransceiverAlert *const alert)
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

TEST_C(AlertValidator, InvalidVariableIdentifierSecondRequirement)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    add_valid_variable_requirement(&(alert.alert_condition));
    /* Invalid variable identifier */
    alert.alert_condition.variable_requirements[1].variable_identifier = 100;
    CHECK_EQUAL_C_ULONG(2, alert.alert_condition.num_variable_requirements);

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, InvalidRequirementOperator)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    /* Invalid operator */
    alert.alert_condition.variable_requirements[0].operator = 55;

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, RequirementOperatorGeqValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].operator = MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_GEQ;

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, RequirementOperatorLeqValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].operator = MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_LEQ;

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, InvalidRequirementOperatorSecondRequirement)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    add_valid_variable_requirement(&(alert.alert_condition));
    /* Invalid variable identifier */
    alert.alert_condition.variable_requirements[1].operator = 0xFF;
    CHECK_EQUAL_C_ULONG(2, alert.alert_condition.num_variable_requirements);

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, LastRequirementIsLastInOredRequirementValid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    add_valid_variable_requirement(&(alert.alert_condition));
    add_valid_variable_requirement(&(alert.alert_condition));
    alert.alert_condition.variable_requirements[2].is_last_in_ored_requirement = true;
    CHECK_EQUAL_C_ULONG(3, alert.alert_condition.num_variable_requirements);

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, LastRequirementIsNotLastInOredRequirementInvalid)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    add_valid_variable_requirement(&(alert.alert_condition));
    add_valid_variable_requirement(&(alert.alert_condition));
    alert.alert_condition.variable_requirements[2].is_last_in_ored_requirement = false;
    CHECK_EQUAL_C_ULONG(3, alert.alert_condition.num_variable_requirements);

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, TemperatureConstraintValueBelowAllowedRange)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier =
        MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_TEMPERATURE;
    alert.alert_condition.variable_requirements[0].constraint_value.temperature = -510;

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, TemperatureConstraintValueAboveAllowedRange)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier =
        MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_TEMPERATURE;
    alert.alert_condition.variable_requirements[0].constraint_value.temperature = 701;

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, TemperatureConstraintValueWithinAllowedRange)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier =
        MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_TEMPERATURE;
    alert.alert_condition.variable_requirements[0].constraint_value.temperature = -500;

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, PressureConstraintValueAboveAllowedRange)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier = MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_PRESSURE;
    alert.alert_condition.variable_requirements[0].constraint_value.pressure = 15001;

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, PressureConstraintValueWithinAllowedRange1)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier = MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_PRESSURE;
    alert.alert_condition.variable_requirements[0].constraint_value.pressure = 15000;

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, PressureConstraintValueWithinAllowedRange2)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier = MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_PRESSURE;
    alert.alert_condition.variable_requirements[0].constraint_value.pressure = 9515;

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, HumidityConstraintValueAboveAllowedRange)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier = MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_HUMIDITY;
    alert.alert_condition.variable_requirements[0].constraint_value.humidity = 1001;

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, HumidityConstraintValueWithinAllowedRange1)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier = MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_HUMIDITY;
    alert.alert_condition.variable_requirements[0].constraint_value.humidity = 1000;

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, HumidityConstraintValueWithinAllowedRange2)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier = MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_HUMIDITY;
    alert.alert_condition.variable_requirements[0].constraint_value.humidity = 555;

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, LightIntensityConstraintValueAboveAllowedRange)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier =
        MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_LIGHT_INTENSITY;
    alert.alert_condition.variable_requirements[0].constraint_value.light_intensity = 130001;

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(!is_valid_alert);
}

TEST_C(AlertValidator, LightIntensityConstraintValueWithinAllowedRange1)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier =
        MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_LIGHT_INTENSITY;
    alert.alert_condition.variable_requirements[0].constraint_value.light_intensity = 130000;

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, LightIntensityConstraintValueWithinAllowedRange2)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier =
        MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_LIGHT_INTENSITY;
    alert.alert_condition.variable_requirements[0].constraint_value.light_intensity = 50453;

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}

TEST_C(AlertValidator, LightIntensityConstraintValueWithinAllowedRange3)
{
    MsgTransceiverAlert alert;
    populate_valid_alert(&alert);
    alert.alert_condition.variable_requirements[0].variable_identifier =
        MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_LIGHT_INTENSITY;
    alert.alert_condition.variable_requirements[0].constraint_value.light_intensity = 0;

    bool is_valid_alert = alert_validator_is_alert_valid(&alert);
    CHECK_C(is_valid_alert);
}
