#ifndef ENV_ALERT_SYSTEM_SRC_APP_MIDDLEWARE_MSG_TRANSCEIVER_H
#define ENV_ALERT_SYSTEM_SRC_APP_MIDDLEWARE_MSG_TRANSCEIVER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "config.h"

#ifndef CONFIG_MSG_TRANSCEIVER_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION
#define CONFIG_MSG_TRANSCEIVER_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION 1
#endif

/* Some type names are prepended with MsgTransceiver to avoid conflicts with type names defined in other modules. */

typedef struct NotificationType {
    uint8_t connectivity : 1;
    uint8_t led : 1;
} NotificationType;

typedef enum MsgTransceiverLedColor {
    MSG_TRANSCEIVER_LED_COLOR_RED = 0,
    MSG_TRANSCEIVER_LED_COLOR_GREEN = 1,
    MSG_TRANSCEIVER_LED_COLOR_BLUE = 2,
} MsgTransceiverLedColor;

typedef enum MsgTransceiverLedPattern {
    MSG_TRANSCEIVER_LED_PATTERN_STATIC = 0,
    MSG_TRANSCEIVER_LED_PATTERN_ALERT = 1,
} MsgTransceiverLedPattern;

typedef enum MsgTransceiverVariableIdentifier {
    MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_TEMPERATURE = 0,
    MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_PRESSURE = 1,
    MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_HUMIDITY = 2,
    MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_LIGHT_INTENSITY = 3,
} MsgTransceiverVariableIdentifier;

typedef enum MsgTransceiverRequirementOperator {
    MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_GEQ = 0,
    MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_LEQ = 1,
} MsgTransceiverRequirementOperator;

typedef int16_t MsgTransceiverTemperature;
typedef uint16_t MsgTransceiverPressure;
typedef uint16_t MsgTransceiverHumidity;
typedef uint16_t MsgTransceiverLightIntensity;

typedef union ConstraintValue {
    MsgTransceiverTemperature temperature;
    MsgTransceiverPressure pressure;
    MsgTransceiverHumidity humidity;
    MsgTransceiverLightIntensity light_intensity;
} ConstraintValue;

typedef struct MsgTransceiverVariableRequirement {
    /**< Should contain values of type enum MsgTransceiverVariableIdentifier */
    uint8_t variable_identifier;
    /**< Should contain values of type enum MsgTransceiverRequirementOperator */
    uint8_t operator;
    /**< variable_identifier field defines which of the union fields should be accessed */
    ConstraintValue constraint_value;
    /** True if it is the last variable requirement in an ORed requirement. An array of type
     * MsgTransceiverVariableRequirement represents an alert condition. If we iterate through the array sequentially,
     * variable requirements that have this field set to true mark the end of an ORed requirement. Each variable
     * requirement that has this field set to true forms an ORed requirement together with all the preceding variable
     * requirements in the array that have this field set to false - until the first variable requirement that has this
     * field set to true, or until the beginning of the array. The last variable requirement in the array is always
     * the last one in an ORed requirement, so it should always have this field set to true. */
    bool is_last_in_ored_requirement;
} MsgTransceiverVariableRequirement;

typedef struct MsgTransceiverAlertCondition {
    /** Number of variable requirements in this alert condition. This is also the number of elements in the
     * variable_requirements array defined below. */
    size_t num_variable_requirements;
    /** Represents an alert condition. Variable requirements that have the is_last_in_ored_requirement field set to true
     * mark the end of an ORed requirement. Each variable requirement that has that field set to true forms an ORed
     * requirement together with all the preceding variable requirements in the array that have that field set to false
     * - until the first variable requirement that has that field set to true, or until the beginning of the array. The
     * last variable requirement in the array is always the last one in an ORed requirement, so it should always have
     * that field set to true. In other words, there is a logical AND (or the end of the array) after every variable
     * requirement that has that field set to true.
     */
    MsgTransceiverVariableRequirement
        variable_requirements[CONFIG_MSG_TRANSCEIVER_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION];
} MsgTransceiverAlertCondition;

typedef struct MsgTransceiverAlert {
    uint8_t alert_id;
    uint32_t warmup_period;
    uint32_t cooldown_period;
    NotificationType notification_type;
    /**< Should contain values of type enum MsgTransceiverLedColor. This field is only applicable if
     * notification_type.led is true. */
    uint8_t led_color;
    /**< Should contain values of type enum MsgTransceiverLedPattern. This field is only applicable if
     * notification_type.led is true. */
    uint8_t led_pattern;
    MsgTransceiverAlertCondition alert_condition;
} MsgTransceiverAlert;

/**
 * @brief Defines callback type to execute when a "add alert" message is received.
 *
 * @param alert Alert to add.
 */
typedef void (*MsgTransceiverAddAlertCb)(const MsgTransceiverAlert *alert);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_MIDDLEWARE_MSG_TRANSCEIVER_H */
