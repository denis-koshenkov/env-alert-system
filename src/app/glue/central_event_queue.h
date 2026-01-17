#ifndef ENV_ALERT_SYSTEM_SRC_APP_GLUE_CENTRAL_EVENT_QUEUE_H
#define ENV_ALERT_SYSTEM_SRC_APP_GLUE_CENTRAL_EVENT_QUEUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "hal/temperature.h"
#include "hal/pressure.h"
#include "hal/humidity.h"
#include "hal/light_intensity.h"

/**
 * @brief Central event queue for event processing.
 *
 * The event queue consists of a thread and a message queue. The thread is blocked on receiving messages from the
 * message queue. Once a message is pushed to the message queue, the thread gets unblocked, pops the message from the
 * queue and handles the event encoded in the message.
 *
 * This event queue is the central place in the system where all the blocking happens. If you are working on a new
 * module and feel the need to introduce some kind of blocking (e.g. on a synchronization primitive, or sleep), use this
 * event queue instead. Blocking is not allowed in any other part of the application.
 *
 * All of the "submit_<event_name>_event" public functions push an event to the event queue and return immediately.
 */

/**
 * @brief Callback type definition for a function without return value and one void* parameter.
 *
 * This callback type definition is used for the "void cb with user data" event. When this event is submitted to the
 * event queue, the handling for this event invokes this callback with user data. Both the callback and the user data
 * are passed as event payload.
 *
 * @param user_data User data.
 */
typedef void (*CentralEventQueueVoidCbWithUserData)(void *user_data);

/**
 * @brief Callback type definition for a function without return value and one uint8_t parameter.
 *
 * This callback type definition is used for the "void cb with uint8" event. When this event is submitted to the event
 * queue, the handling for this event invokes this callback with uint8_t parameter. Both the callback and its argument
 * are passed as event payload.
 *
 * @param param_uint8 uint8_t argument to pass to the callback.
 */
typedef void (*CentralEventQueueVoidCbWithUint8)(uint8_t param_uint8);

/**
 * @brief Initialize central event queue.
 *
 * Creates the message queue and the thread. The thread is immediately started. This function should be called on system
 * startup.
 */
void central_event_queue_init();

/**
 * @brief Submit init event to the event queue.
 */
void central_event_queue_submit_init_event();

/**
 * @brief Submit init part 2 event to the event queue.
 */
void central_event_queue_submit_init_part_2_event();

/**
 * @brief Submit new temperature sample event to the event queue.
 *
 * @param temperature New temperature sample value.
 */
void central_event_queue_submit_new_temperature_sample_event(Temperature temperature);

/**
 * @brief Submit new pressure sample event to the event queue.
 *
 * @param pressure New pressure sample value.
 */
void central_event_queue_submit_new_pressure_sample_event(Pressure pressure);

/**
 * @brief Submit new humidity sample event to the event queue.
 *
 * @param humidity New humidity sample value.
 */
void central_event_queue_submit_new_humidity_sample_event(Humidity humidity);

/**
 * @brief Submit new light intensity sample event to the event queue.
 *
 * @param light_intensity New light intensity sample value.
 */
void central_event_queue_submit_new_light_intensity_sample_event(LightIntensity light_intensity);

/**
 * @brief Submit void cb with user data event to the event queue.
 *
 * The handler for this event will execute @p cb and pass @p user_data as the parameter to @p cb.
 *
 * @param cb Callback to execute. Cannot be NULL.
 * @param user_data User data to pass to the callback.
 */
void central_event_queue_submit_void_cb_with_user_data_event(CentralEventQueueVoidCbWithUserData cb, void *user_data);

/**
 * @brief Submit void cb with uint8 event to the event queue.
 *
 * The handler for this event will execute @p cb and pass @p param_uint8 parameter to @p cb.
 *
 * @param cb Callback to execute. Cannot be NULL.
 * @param param_uint8 uint8_t parameter to a pass as the first parameter to the callback.
 */
void central_event_queue_submit_void_cb_with_uint8_event(CentralEventQueueVoidCbWithUint8 cb, uint8_t param_uint8);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_GLUE_CENTRAL_EVENT_QUEUE_H */
