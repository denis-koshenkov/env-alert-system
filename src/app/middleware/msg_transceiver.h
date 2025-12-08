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

/**
 * @brief Sends and receives application protocol messages.
 *
 * The main responsibility of this module is to convert between structured data and raw bytes of application protocol
 * messages. Whenever application protocol messages are received, this module parses the received bytes, and if the
 * payload structure is valid, converts the raw bytes to a structured representation of the payload.
 *
 * Whenever an application protocol message needs to be sent, this module converts structured data into raw bytes and
 * transmits the bytes.
 *
 * # Usage
 *
 * ```
 * // Initialize the module
 * msg_transceiver_init();
 * // Register callbacks to execute whenever "add alert" and "remove alert" messages are received
 * msg_transceiver_set_add_alert_cb(add_alert_cb, add_alert_cb_user_data);
 * msg_transceiver_set_remove_alert_cb(remove_alert_cb, remove_alert_cb_user_data);
 *
 * // Send "alert status change" message whenever needed
 * msg_transceiver_send_alert_status_change_message(alert_id, is_raised, cb, user_data);
 * ```
 */

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
typedef uint32_t MsgTransceiverLightIntensity;

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
 * @brief Defines callback type to execute when a message has been sent.
 *
 * @param result True if message was sent successfully, false if failed to send message.
 * @param user_data User data.
 */
typedef void (*MsgTransceiverMessageSentCb)(bool result, void *user_data);

/**
 * @brief Defines callback type to execute when a "add alert" message is received.
 *
 * @param alert Alert to add.
 * @param user_data User data.
 */
typedef void (*MsgTransceiverAddAlertCb)(const MsgTransceiverAlert *const alert, void *user_data);

/**
 * @brief Defines callback type to execute when a "remove alert" message is received.
 *
 * @param alert_id Id of alert to remove.
 * @param user_data User data.
 */
typedef void (*MsgTransceiverRemoveAlertCb)(uint8_t alert_id, void *user_data);

/**
 * @brief Initialize message transceiver module.
 *
 * This function should be called before any other function in this module. Raises an assert if the module is already
 * initialized.
 */
void msg_transceiver_init();

/**
 * @brief Send alert status change message.
 *
 * @pre Module has been initialized by calling @ref msg_transceiver_init.
 *
 * @param alert_id Alert id.
 * @param is_raised True if alert status changed to "raised", false if alert status changed to "silenced".
 * @param cb Callback to execute once the message is sent.
 * @param user_data User data to pass to @p cb as a parameter.
 */
void msg_transceiver_send_alert_status_change_message(uint8_t alert_id, bool is_raised, MsgTransceiverMessageSentCb cb,
                                                      void *user_data);

/**
 * @brief Set callback to execute whenever a "add alert" message is received.
 *
 * @pre Module has been initialized by calling @ref msg_transceiver_init.
 *
 * @param cb Callback to execute.
 * @param user_data User data to pass to @p cb as a parameter.
 */
void msg_transceiver_set_add_alert_cb(MsgTransceiverAddAlertCb cb, void *user_data);

/**
 * @brief Set callback to execute whenever a "remove alert" message is received.
 *
 * @pre Module has been initialized by calling @ref msg_transceiver_init.
 *
 * @param cb Callback to execute.
 * @param user_data User data to pass to @p cb as a parameter.
 */
void msg_transceiver_set_remove_alert_cb(MsgTransceiverRemoveAlertCb cb, void *user_data);

/**
 * @brief Deinitialize message transceiver module.
 *
 * Does nothing if it is already in the deinitialized state.
 *
 * If this function is called when one or more alert status messages are in the process of being sent, their "message
 * sent" callbacks will not be executed anymore. "In the process of being sent" means that @ref
 * msg_transceiver_send_alert_status_change_message has been called, but the callback passed as a parameter to that
 * function has not yet been executed.
 */
void msg_transceiver_deinit();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_MIDDLEWARE_MSG_TRANSCEIVER_H */
