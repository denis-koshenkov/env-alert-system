#include <stddef.h>

#include "eas_assert.h"
#include "alert_adder.h"
#include "alert_raisers.h"
#include "alert_raiser.h"
#include "connectivity_notifier.h"
#include "led_notifier.h"
#include "alert_conditions.h"
#include "alert_condition.h"
#include "temperature_requirement.h"
#include "pressure_requirement.h"
#include "humidity_requirement.h"
#include "light_intensity_requirement.h"
#include "temperature_requirement_list.h"
#include "pressure_requirement_list.h"
#include "humidity_requirement_list.h"
#include "light_intensity_requirement_list.h"

/**
 * @brief Map LED color from message transceiver to led color from LED HAL module.
 *
 * @param led_color Message transceiver led color.
 *
 * @return LedColor Corresponding HAL LED color. If @p led_color is invalid, an assert is raised.
 */
static LedColor map_msg_transceiver_led_color_to_led_color(MsgTransceiverLedColor led_color)
{
    switch (led_color) {
    case MSG_TRANSCEIVER_LED_COLOR_RED:
        return LED_COLOR_RED;
    case MSG_TRANSCEIVER_LED_COLOR_GREEN:
        return LED_COLOR_GREEN;
    case MSG_TRANSCEIVER_LED_COLOR_BLUE:
        return LED_COLOR_BLUE;
    default:
        EAS_ASSERT(0);
        return LED_COLOR_RED;
    }
}

/**
 * @brief Map LED pattern from message transceiver to led pattern from LED HAL module.
 *
 * @param led_pattern Message transceiver led pattern.
 *
 * @return LedPattern Corresponding HAL LED pattern. If @p led_pattern is invalid, an assert is raised.
 */
static LedPattern map_msg_transceiver_led_pattern_to_led_pattern(MsgTransceiverLedPattern led_pattern)
{
    switch (led_pattern) {
    case MSG_TRANSCEIVER_LED_PATTERN_STATIC:
        return LED_PATTERN_STATIC;
    case MSG_TRANSCEIVER_LED_PATTERN_ALERT:
        return LED_PATTERN_ALERT;
    default:
        EAS_ASSERT(0);
        return LED_PATTERN_STATIC;
    }
}

/**
 * @brief Map requirement operator from message transceiver to requirement operator from variable requirement module.
 *
 * @param operator Message transceiver requirement operator.
 *
 * @return LedPattern Corresponding requirement operator from variable requirement module. If @p operator is invalid,
 * an assert is raised.
 */
static VariableRequirementOperator
map_msg_transceiver_operator_to_variable_requirement_operator(MsgTransceiverRequirementOperator operator)
{
    switch (operator) {
    case MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_GEQ:
        return VARIABLE_REQUIREMENT_OPERATOR_GEQ;
    case MSG_TRANSCEIVER_REQUIREMENT_OPERATOR_LEQ:
        return VARIABLE_REQUIREMENT_OPERATOR_LEQ;
    default:
        EAS_ASSERT(0);
        return VARIABLE_REQUIREMENT_OPERATOR_GEQ;
    }
}

void alert_adder_add_alert(const MsgTransceiverAlert *alert)
{
    EAS_ASSERT(alert);

    /* TODO: validate alert. Decide if we do it here or in msg_transceiver. Probably here, because msg_transceiver is
     * responsible only for delivering the message, not for application-specific details on what the payload should look
     * like. */

    AlertRaiser alert_raiser = alert_raisers_get_alert_raiser(alert->alert_id);
    if (alert_raiser_is_alert_set(alert_raiser)) {
        /* Alert id for which we are trying to add the alert is already occupied - cannot add alert */
        return;
    }

    /* Enable required notifications for the alert*/
    if (alert->notification_type.connectivity) {
        connectivity_notifier_enable_notifications(alert->alert_id);
    }
    if (alert->notification_type.led) {
        LedColor led_color = map_msg_transceiver_led_color_to_led_color(alert->led_color);
        LedPattern led_pattern = map_msg_transceiver_led_pattern_to_led_pattern(alert->led_pattern);
        led_notifier_enable_notifications(alert->alert_id, led_color, led_pattern);
    }

    /* Already checked above that an alert is not set for this alert raiser, so it is safe to call this function. */
    alert_raiser_set_alert(alert_raiser, alert->alert_id, alert->warmup_period, alert->cooldown_period);

    /* Populate alert condition */
    AlertCondition alert_condition = alert_conditions_get_alert_condition(alert->alert_id);
    alert_condition_reset(alert_condition);
    for (size_t i = 0; i < alert->alert_condition.num_variable_requirements; i++) {
        const MsgTransceiverVariableRequirement *variable_requirement =
            &(alert->alert_condition.variable_requirements[i]);
        VariableRequirementOperator operator =
            map_msg_transceiver_operator_to_variable_requirement_operator(variable_requirement->operator);

        /* Create a new variable requirement instance and add it to the list of variable requirements of that type */
        VariableRequirement new_variable_requirement;
        switch (variable_requirement->variable_identifier) {
        case MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_TEMPERATURE:
            new_variable_requirement = temperature_requirement_create(
                alert->alert_id, operator, (Temperature)variable_requirement->constraint_value.temperature);
            temperature_requirement_list_add(new_variable_requirement);
            break;
        case MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_PRESSURE:
            new_variable_requirement = pressure_requirement_create(
                alert->alert_id, operator, (Pressure)variable_requirement->constraint_value.pressure);
            pressure_requirement_list_add(new_variable_requirement);
            break;
        case MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_HUMIDITY:
            new_variable_requirement = humidity_requirement_create(
                alert->alert_id, operator, (Humidity)variable_requirement->constraint_value.humidity);
            humidity_requirement_list_add(new_variable_requirement);
            break;
        case MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_LIGHT_INTENSITY:
            new_variable_requirement = light_intensity_requirement_create(
                alert->alert_id, operator, (LightIntensity)variable_requirement->constraint_value.light_intensity);
            light_intensity_requirement_list_add(new_variable_requirement);
            break;
        }

        /* Add variable requirement to the alert condition for this alert */
        alert_condition_add_variable_requirement(alert_condition, new_variable_requirement);
        if (variable_requirement->is_last_in_ored_requirement) {
            /* This will always be called after the last variable requirement is added to the alert condition. There is
             * no harm in doing that, even though it is not necessary. */
            alert_condition_start_new_ored_requirement(alert_condition);
        }
    }

    /* TODO: evaluate alert condition for the first time, if we received a sample for each of the variables from
     * hardware. */
}
