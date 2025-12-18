#include "init_handler.h"
#include "alert_conditions.h"
#include "alert_raisers.h"
#include "alert_adder.h"
#include "alert_remover.h"
#include "msg_transceiver.h"
#include "eas_timer.h"
#include "eas_timer_callback_executor.h"
#include "hw_platform.h"
#include "new_sample_callbacks.h"

void init_handler_handle_init_event()
{
    /* Done before initializing hardware platform, in case hw_platform_init starts timers. */
    eas_timer_set_execute_timer_expiry_function_cb(eas_timer_callback_executor_execute_callback);

    hw_platform_init();
    hw_platform_get_temperature_sensor()->register_new_sample_cb(new_sample_callback_temperature, NULL);
    hw_platform_get_pressure_sensor()->register_new_sample_cb(new_sample_callback_pressure, NULL);
    hw_platform_get_humidity_sensor()->register_new_sample_cb(new_sample_callback_humidity, NULL);
    hw_platform_get_light_intensity_sensor()->register_new_sample_cb(new_sample_callback_light_intensity, NULL);
    hw_platform_get_temperature_sensor()->start();
    hw_platform_get_pressure_sensor()->start();
    hw_platform_get_humidity_sensor()->start();
    hw_platform_get_light_intensity_sensor()->start();

    alert_conditions_create_instances();
    alert_raisers_create_instances();
    msg_transceiver_init();
    msg_transceiver_set_add_alert_cb(alert_adder_add_alert, NULL);
    msg_transceiver_set_remove_alert_cb(alert_remover_remove_alert, NULL);
}
