#include "new_sample_handler.h"
#include "alert_evaluation_readiness.h"
#include "current_temperature.h"
#include "current_pressure.h"
#include "current_humidity.h"
#include "current_light_intensity.h"
#include "temperature_requirement_list.h"
#include "pressure_requirement_list.h"
#include "humidity_requirement_list.h"
#include "light_intensity_requirement_list.h"
#include "variable_requirement.h"
#include "alert_conditions.h"
#include "alert_condition.h"
#include "alert_raisers.h"
#include "alert_raiser.h"
#include "eas_assert.h"
#include "eas_log.h"

EAS_LOG_ENABLE_IN_FILE();

/**
 * @brief Evaluate variable requirement and if its result changed, also evaluate the alert condition it is a part of.
 *
 * Callback to execute for every variable requirement in a variable requirement list.
 *
 * @param variable_requirement Variable requirement to evaluate.
 */
static void evaluate_variable_requirement(VariableRequirement variable_requirement)
{
    /* Do not care about return value, evaluate is called to trigger result update, so that we can check whether
     * result changed */
    variable_requirement_evaluate(variable_requirement);
    if (variable_requirement_is_result_changed(variable_requirement)) {
        uint8_t alert_id = variable_requirement_get_alert_id(variable_requirement);
        AlertCondition alert_condition = alert_conditions_get_alert_condition(alert_id);
        bool condition_result = alert_condition_evaluate(alert_condition);
        AlertRaiser alert_raiser = alert_raisers_get_alert_raiser(alert_id);
        alert_raiser_set_alert_condition_result(alert_raiser, condition_result);
    }
}

/**
 * @brief Callback to execute when the current temperature value has changed.
 */
static void handle_temperature_value_change()
{
    temperature_requirement_list_for_each(evaluate_variable_requirement);
}

/**
 * @brief Callback to execute when the current pressure value has changed.
 */
static void handle_pressure_value_change()
{
    pressure_requirement_list_for_each(evaluate_variable_requirement);
}

/**
 * @brief Callback to execute when the current humidity value has changed.
 */
static void handle_humidity_value_change()
{
    humidity_requirement_list_for_each(evaluate_variable_requirement);
}

/**
 * @brief Callback to execute when the current light intensity value has changed.
 */
static void handle_light_intensity_value_change()
{
    light_intensity_requirement_list_for_each(evaluate_variable_requirement);
}

/**
 * @brief Callback implementation to set current temperature value.
 *
 * @param sample Pointer to temperature sample, should point to data of type Temperature.
 */
static void set_current_temperature_value(const void *const sample)
{
    EAS_ASSERT(sample);
    const Temperature *const temperature = (const Temperature *const)sample;
    current_temperature_set(*temperature);
}

/**
 * @brief Callback implementation to set current pressure value.
 *
 * @param sample Pointer to pressure sample, should point to data of type Pressure.
 */
static void set_current_pressure_value(const void *const sample)
{
    EAS_ASSERT(sample);
    const Pressure *const pressure = (const Pressure *const)sample;
    current_pressure_set(*pressure);
}

/**
 * @brief Callback implementation to set current humidity value.
 *
 * @param sample Pointer to humidity sample, should point to data of type Humidity.
 */
static void set_current_humidity_value(const void *const sample)
{
    EAS_ASSERT(sample);
    const Humidity *const humidity = (const Humidity *const)sample;
    current_humidity_set(*humidity);
}

/**
 * @brief Callback implementation to set current light intensity value.
 *
 * @param sample Pointer to light intensity sample, should point to data of type LightIntensity.
 */
static void set_current_light_intensity_value(const void *const sample)
{
    EAS_ASSERT(sample);
    const LightIntensity *const light_intensity = (const LightIntensity *const)sample;
    current_light_intensity_set(*light_intensity);
}

/**
 * @brief Generic new sample handler.
 *
 * @param sample Pointer to the sample value. This pointer will be passed to the @p set_current_sample_value callback
 * function as a parameter. This pointer is not used for anything else.
 * @param notify_alert_evaluation_readiness The implementation of this callback should notify the alert evaluation
 * readiness module that a sample of this variable type has been received. The caller of this function knows the exact
 * variable for which this generic handler is being invoked.
 * @param set_current_sample_value This callback should set the new sample value to the current_<variable name> module.
 * @p sample is passed as a parameter to this callback function.
 * @param handle_sample_value_change The implementation of this callback function should update the alert condition
 * result of all alert conditions that contain a variable requirement of this variable type.
 */
static void new_sample_handler(const void *const sample, void (*notify_alert_evaluation_readiness)(),
                               void (*set_current_sample_value)(const void *const sample),
                               void (*handle_sample_value_change)())
{
    EAS_ASSERT(sample);
    EAS_ASSERT(notify_alert_evaluation_readiness);
    EAS_ASSERT(set_current_sample_value);
    EAS_ASSERT(handle_sample_value_change);

    bool is_ready_before = alert_evaluation_readiness_is_ready();
    notify_alert_evaluation_readiness();
    bool is_ready_after = alert_evaluation_readiness_is_ready();
    set_current_sample_value(sample);

    if (!is_ready_after) {
        /* Samples of some variables have not been received yet - cannot evaluate alert conditions */
        return;
    }

    if (!is_ready_before && is_ready_after) {
        /* This is the sample that makes alert_evaluation_readiness ready to start evaluating alert conditions. Evaluate
         * ALL variable requirements and alert conditions, not only the ones for this variable. */
        handle_temperature_value_change();
        handle_pressure_value_change();
        handle_humidity_value_change();
        handle_light_intensity_value_change();
    } else if (current_temperature_is_changed()) {
        handle_sample_value_change();
    }
}

void new_sample_handler_temperature(Temperature temperature)
{
    EAS_LOG_INF("New temperature sample %d", temperature);
    new_sample_handler(&temperature, alert_evaluation_readiness_notify_received_temperature_sample,
                       set_current_temperature_value, handle_temperature_value_change);
}

void new_sample_handler_pressure(Pressure pressure)
{
    EAS_LOG_INF("New pressure sample %d", pressure);
    new_sample_handler(&pressure, alert_evaluation_readiness_notify_received_pressure_sample,
                       set_current_pressure_value, handle_pressure_value_change);
}

void new_sample_handler_humidity(Humidity humidity)
{
    EAS_LOG_INF("New humidity sample %d", humidity);
    new_sample_handler(&humidity, alert_evaluation_readiness_notify_received_humidity_sample,
                       set_current_humidity_value, handle_humidity_value_change);
}

void new_sample_handler_light_intensity(LightIntensity light_intensity)
{
    EAS_LOG_INF("New light intensity sample %d", light_intensity);
    new_sample_handler(&light_intensity, alert_evaluation_readiness_notify_received_light_intensity_sample,
                       set_current_light_intensity_value, handle_light_intensity_value_change);
}
