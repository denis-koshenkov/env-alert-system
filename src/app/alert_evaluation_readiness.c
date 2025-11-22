#include <stddef.h>

#include "alert_evaluation_readiness.h"
#include "eas_assert.h"

static bool temperature_sample_received = false;
static bool pressure_sample_received = false;
static bool humidity_sample_received = false;
static bool light_intensity_sample_received = false;
static AlertEvaluationReadinessReadyCb ready_cb = NULL;

/**
 * @brief Check whether at least one sample for each of the variables has been received.
 *
 * @return true At least one sample for each of the variables has been received.
 * @return false There is at least one variable for which a sample has not been received yet.
 */
static bool all_samples_received()
{
    // clang-format off
    return (
        temperature_sample_received
        && pressure_sample_received
        && humidity_sample_received
        && light_intensity_sample_received
    );
    // clang-format on
}

/**
 * @brief Execute the registered ready cb, if one was set and at least one sample for each variable has been received.
 */
static void execute_cb_if_all_samples_received()
{
    if (all_samples_received()) {
        if (ready_cb) {
            ready_cb();
        }
    }
}

bool alert_evaluation_readiness_is_ready()
{
    return all_samples_received();
}

void alert_evaluation_readiness_notify_received_temperature_sample()
{
    temperature_sample_received = true;
    execute_cb_if_all_samples_received();
}

void alert_evaluation_readiness_notify_received_pressure_sample()
{
    pressure_sample_received = true;
    execute_cb_if_all_samples_received();
}

void alert_evaluation_readiness_notify_received_humidity_sample()
{
    humidity_sample_received = true;
    execute_cb_if_all_samples_received();
}

void alert_evaluation_readiness_notify_received_light_intensity_sample()
{
    light_intensity_sample_received = true;
    execute_cb_if_all_samples_received();
}

void alert_evaluation_readiness_set_ready_cb(AlertEvaluationReadinessReadyCb cb)
{
    EAS_ASSERT(cb);
    ready_cb = cb;
}

void alert_evaluation_readiness_reset()
{
    temperature_sample_received = false;
    pressure_sample_received = false;
    humidity_sample_received = false;
    light_intensity_sample_received = false;
    ready_cb = NULL;
}
