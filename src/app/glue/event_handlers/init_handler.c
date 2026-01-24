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
#include "central_event_queue.h"
#include "eas_log.h"

EAS_LOG_ENABLE_IN_FILE();

/* System initialization sequence:
 * 1. main function submits INIT event to the event queue, which triggers the execution of
 * init_handler_handle_init_event.
 * 2. init_handler_handle_init_event performs the first part of system initialization. It triggers the hardware platform
 * initialization and returns.
 * 3. Hardware platform init complete callback is executed, which is the hw_platform_init_complete_cb function defined
 * in this module. Inside this callback, we submit the INIT_PART_2 event to the central event queue. This triggers the
 * init_handler_handle_init_part_2_event function to be called.
 * 4. init_handler_handle_init_part_2_event executes part 2 of the system initialization. Initialization is now
 * complete.
 */

/**
 * @brief Callback to execute once hw platform initialization is complete.
 *
 * Submits an event to the event queue which triggers the execution of part 2 of system initialization.
 *
 * @param result True if hw platform init was successful, false otherwise.
 * @param user_data User data, unused.
 */
static void hw_platform_init_complete_cb(bool result, void *user_data)
{
    if (result) {
        central_event_queue_submit_init_part_2_event();
    } else {
        LOG_INF("Hardware init failed");
    }
}

void init_handler_handle_init_event()
{
    /* Done before initializing hardware platform, in case hw_platform_init starts timers. */
    eas_timer_set_execute_timer_expiry_function_cb(eas_timer_callback_executor_execute_callback);

    hw_platform_init(hw_platform_init_complete_cb, NULL);
}

void init_handler_handle_init_part_2_event()
{
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
