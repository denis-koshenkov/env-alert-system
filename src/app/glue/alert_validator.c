#include "alert_validator.h"
#include "config.h"

#ifndef CONFIG_ALERT_VALIDATOR_MAX_ALLOWED_ALERT_ID
#define CONFIG_ALERT_VALIDATOR_MAX_ALLOWED_ALERT_ID 0
#endif

#define ALERT_VALIDATOR_MIN_TEMPERATURE_CONSTRAINT_VALUE -500       // -50.0 degrees Celsius
#define ALERT_VALIDATOR_MAX_TEMPERATURE_CONSTRAINT_VALUE 700        // 70.0 degrees Celsius
#define ALERT_VALIDATOR_MAX_PRESSURE_CONSTRAINT_VALUE 15000         // 1500.0 hPa
#define ALERT_VALIDATOR_MAX_HUMIDITY_CONSTRAINT_VALUE 1000          // 100.0 %
#define ALERT_VALIDATOR_MAX_LIGHT_INTENSITY_CONSTRAINT_VALUE 130000 // 130,000 lux

/**
 * @brief Check whether alert id is valid.
 *
 * @param alert_id Alert id.
 *
 * @return true Alert id is valid.
 * @return false Alert id is invalid.
 */
static bool is_alert_id_valid(uint8_t alert_id)
{
    return (alert_id <= CONFIG_ALERT_VALIDATOR_MAX_ALLOWED_ALERT_ID);
}

/**
 * @brief Check whether LED color of alert notification is valid.
 *
 * @param led_color Led color.
 *
 * @return true @p led_color is one of the valid values from @ref MsgTransceiverLedColor.
 * @return false @p led_color is not a valid value from @ref MsgTransceiverLedColor.
 */
static bool is_led_color_valid(uint8_t led_color)
{
    // clang-format off
    return (
        (led_color == MSG_TRANSCEIVER_LED_COLOR_RED)
        || (led_color == MSG_TRANSCEIVER_LED_COLOR_GREEN)
        || (led_color == MSG_TRANSCEIVER_LED_COLOR_BLUE)
    );
    // clang-format on
}

/**
 * @brief Check whether LED pattern of alert notification is valid.
 *
 * @param led_pattern Led pattern.
 *
 * @return true @p led_pattern is one of the valid values from @ref MsgTransceiverLedPattern.
 * @return false @p led_pattern is not a valid value from @ref MsgTransceiverLedPattern.
 */
static bool is_led_pattern_valid(uint8_t led_pattern)
{
    // clang-format off
    return (
        (led_pattern == MSG_TRANSCEIVER_LED_PATTERN_STATIC)
        || (led_pattern == MSG_TRANSCEIVER_LED_PATTERN_ALERT)
    );
    // clang-format on
}

/**
 * @brief Check whether notification type of alert is valid.
 *
 * @param notification_type Notification type.
 *
 * @return true Notification type is valid.
 * @return false Notification type is invalid.
 */
static bool is_notification_type_valid(NotificationType notification_type)
{
    /* At least one notification type should be enabled, otherwise there is no point in adding this alert - nothing will
     * happen when it gets raised. */
    return !((notification_type.connectivity == 0) && (notification_type.led == 0));
}

/**
 * @brief Check whether variable identifier is valid.
 *
 * @param variable_identifier Variable identifier. Use values from enum @ref MsgTransceiverVariableIdentifier.
 *
 * @return true Variable identifier is valid.
 * @return false Variable identifier is invalid.
 */
static bool is_valid_variable_identifier(uint8_t variable_identifier)
{
    // clang-format off
    return (
        (variable_identifier == MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_TEMPERATURE)
        || (variable_identifier == MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_PRESSURE)
        || (variable_identifier == MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_HUMIDITY)
        || (variable_identifier == MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_LIGHT_INTENSITY)
    );
    // clang-format on
}

/**
 * @brief Check whether variable requirement operator is valid.
 *
 * @param operator Operator. Use values from enum @ref MsgTransceiverRequirementOperator.
 *
 * @return true Operator is valid.
 * @return false Operator is invalid.
 */
static bool is_valid_operator(uint8_t operator)
{
    return (operator == MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_GEQ) ||
           (operator == MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_LEQ);
}

/**
 * @brief Check that constraint value is within range.
 *
 * @param variable_identifier Variable identifier.
 * @param constraint_value Constraint value to validate.
 *
 * @return true Constraint value is within allowed range for the variable.
 * @return false Constraint value is outside of the allowed range for the variable.
 */
static bool is_valid_constraint_value(uint8_t variable_identifier, ConstraintValue constraint_value)
{
    bool is_valid = false;
    switch (variable_identifier) {
    case MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_TEMPERATURE:
        is_valid = (constraint_value.temperature >= ALERT_VALIDATOR_MIN_TEMPERATURE_CONSTRAINT_VALUE) &&
                   (constraint_value.temperature <= ALERT_VALIDATOR_MAX_TEMPERATURE_CONSTRAINT_VALUE);
        break;
    case MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_PRESSURE:
        is_valid = (constraint_value.pressure <= ALERT_VALIDATOR_MAX_PRESSURE_CONSTRAINT_VALUE);
        break;
    case MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_HUMIDITY:
        is_valid = (constraint_value.humidity <= ALERT_VALIDATOR_MAX_HUMIDITY_CONSTRAINT_VALUE);
        break;
    case MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_LIGHT_INTENSITY:
        is_valid = (constraint_value.light_intensity <= ALERT_VALIDATOR_MAX_LIGHT_INTENSITY_CONSTRAINT_VALUE);
        break;
    default:
        /* Invalid variable identifier */
        is_valid = false;
        break;
    }

    return is_valid;
}

/**
 * @brief Check whether alert condition is valid.
 *
 * @param alert_condition Alert condition.
 *
 * @return true Alert condition is valid.
 * @return false Alert condition is invalid.
 */
static bool is_alert_condition_valid(const MsgTransceiverAlertCondition *const alert_condition)
{
    bool valid_num_variable_requirements = (alert_condition->num_variable_requirements != 0);

    bool all_variable_identifiers_valid = true;
    bool all_operators_valid = true;
    bool all_constraint_values_valid =
        is_valid_constraint_value(alert_condition->variable_requirements[0].variable_identifier,
                                  alert_condition->variable_requirements[0].constraint_value);
    for (size_t i = 0; i < alert_condition->num_variable_requirements; i++) {
        if (!is_valid_variable_identifier(alert_condition->variable_requirements[i].variable_identifier)) {
            all_variable_identifiers_valid = false;
            break;
        }
        if (!is_valid_operator(alert_condition->variable_requirements[i].operator)) {
            all_operators_valid = false;
            break;
        }
    }

    size_t last_requirement_index = alert_condition->num_variable_requirements - 1;
    bool last_requirement_is_last_in_ored_requirement =
        (alert_condition->variable_requirements[last_requirement_index].is_last_in_ored_requirement == true);

    return (valid_num_variable_requirements && all_variable_identifiers_valid && all_operators_valid &&
            all_constraint_values_valid && last_requirement_is_last_in_ored_requirement);
}

bool alert_validator_is_alert_valid(const MsgTransceiverAlert *alert)
{
    /* LED color and pattern are allowed to have invalid values if led notification is disabled, since these values will
     * not be used. */
    bool led_color_pattern_valid = ((alert->notification_type.led == 0) ||
                                    (is_led_color_valid(alert->led_color) && is_led_pattern_valid(alert->led_pattern)));
    // clang-format off
    return (
        is_alert_id_valid(alert->alert_id)
        && led_color_pattern_valid
        && is_notification_type_valid(alert->notification_type)
        && is_alert_condition_valid(&(alert->alert_condition))
    );
    // clang-format on
}
