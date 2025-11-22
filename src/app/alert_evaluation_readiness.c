#include "alert_evaluation_readiness.h"

static bool temperature_sample_received = false;
static bool pressure_sample_received = false;
static bool humidity_sample_received = false;
static bool light_intensity_sample_received = false;

bool alert_evaluation_readiness_is_ready()
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

void alert_evaluation_readiness_notify_received_temperature_sample()
{
    temperature_sample_received = true;
}

void alert_evaluation_readiness_notify_received_pressure_sample()
{
    pressure_sample_received = true;
}

void alert_evaluation_readiness_notify_received_humidity_sample()
{
    humidity_sample_received = true;
}

void alert_evaluation_readiness_notify_received_light_intensity_sample()
{
    light_intensity_sample_received = true;
}

void alert_evaluation_readiness_reset()
{
    temperature_sample_received = false;
    pressure_sample_received = false;
    humidity_sample_received = false;
    light_intensity_sample_received = false;
}
