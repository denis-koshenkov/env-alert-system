#include "new_sample_callbacks.h"
#include "central_event_queue.h"

void new_sample_callback_temperature(Temperature sample, void *user_data)
{
    central_event_queue_submit_new_temperature_sample_event(sample);
}

void new_sample_callback_pressure(Pressure sample, void *user_data)
{
    central_event_queue_submit_new_pressure_sample_event(sample);
}

void new_sample_callback_humidity(Humidity sample, void *user_data)
{
    central_event_queue_submit_new_humidity_sample_event(sample);
}

void new_sample_callback_light_intensity(LightIntensity sample, void *user_data)
{
    central_event_queue_submit_new_light_intensity_sample_event(sample);
}
