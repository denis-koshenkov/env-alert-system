#include <stdint.h>

#include "central_event_queue.h"
#include "osal/eas_message_queue.h"
#include "osal/eas_thread.h"
#include "hal/temperature.h"
#include "eas_assert.h"
#include "utils/util.h"
#include "new_sample_handler.h"
#include "config.h"
#include "init_handler.h"

#ifndef CONFIG_CENTRAL_EVENT_QUEUE_MESSAGE_QUEUE_BUF_SIZE
#define CONFIG_CENTRAL_EVENT_QUEUE_MESSAGE_QUEUE_BUF_SIZE 1024
#endif

typedef struct CentralEventQueue {
    EasMessageQueue message_queue;
    uint8_t message_queue_buf[CONFIG_CENTRAL_EVENT_QUEUE_MESSAGE_QUEUE_BUF_SIZE];
} CentralEventQueue;

/** Unique event identifiers. This is the first byte of every event message to signal what kind of event it is. */
typedef enum EventId {
    EVENT_ID_INIT = 0,
    EVENT_ID_NEW_TEMPERATURE_SAMPLE,
    EVENT_ID_NEW_PRESSURE_SAMPLE,
    EVENT_ID_NEW_HUMIDITY_SAMPLE,
    EVENT_ID_NEW_LIGHT_INTENSITY_SAMPLE,
    /** Handler for this event executes a callback function provided in the event payload. User data parameter to pass
     * to the callback is also a part of the payload. */
    EVENT_ID_VOID_CB_WITH_USER_DATA,
} EventId;

/** Abstract event class that includes only event id. Events that have payload should put this struct as the first field
 * in their struct, so that the first byte of each event is always the event id. */
typedef struct Event {
    uint8_t id;
} Event;

typedef struct NewTemperatureSampleEvent {
    Event event;
    Temperature sample;
} NewTemperatureSampleEvent;

typedef struct NewPressureSampleEvent {
    Event event;
    Pressure sample;
} NewPressureSampleEvent;

typedef struct NewHumiditySampleEvent {
    Event event;
    Humidity sample;
} NewHumiditySampleEvent;

typedef struct NewLightIntensitySampleEvent {
    Event event;
    LightIntensity sample;
} NewLightIntensitySampleEvent;

typedef struct VoidCbWithUserDataEvent {
    Event event;
    CentralEventQueueVoidCbWithUserData cb;
    void *user_data;
} VoidCbWithUserDataEvent;

/* Wrapping in an enum, so that the value is a constant, and we can declare an array of this size */
enum {
    CENTRAL_EVENT_QUEUE_MAX_MESSAGE_SIZE =
        MAX5(sizeof(NewTemperatureSampleEvent), sizeof(NewPressureSampleEvent), sizeof(NewHumiditySampleEvent),
             sizeof(NewLightIntensitySampleEvent), sizeof(VoidCbWithUserDataEvent))
};

static CentralEventQueue self;

static void handle_init_event()
{
    init_handler_handle_init_event();
}

static void handle_new_temperature_sample_event(const NewTemperatureSampleEvent *const event)
{
    EAS_ASSERT(event);
    new_sample_handler_temperature(event->sample);
}

static void handle_new_pressure_sample_event(const NewPressureSampleEvent *const event)
{
    EAS_ASSERT(event);
    new_sample_handler_pressure(event->sample);
}

static void handle_new_humidity_sample_event(const NewHumiditySampleEvent *const event)
{
    EAS_ASSERT(event);
    new_sample_handler_humidity(event->sample);
}

static void handle_new_light_intensity_sample_event(const NewLightIntensitySampleEvent *const event)
{
    EAS_ASSERT(event);
    new_sample_handler_light_intensity(event->sample);
}

/**
 * @brief Handle "void cb with user data" event by invoking the callback.
 *
 * @param event "Void cb with user data" event.
 */
static void handle_void_cb_with_user_data_event(const VoidCbWithUserDataEvent *const event)
{
    EAS_ASSERT(event);
    EAS_ASSERT(event->cb);
    event->cb(event->user_data);
}

/**
 * @brief Central event queue thread run function.
 *
 * Blocks on the event message queue. When a message is received, pops it from the queue and calls the corresponding
 * event handler.
 */
static void central_event_queue_thread_run()
{
    static uint8_t message[CENTRAL_EVENT_QUEUE_MAX_MESSAGE_SIZE];
    while (1) {
        /* This blocks until a message is pushed to the message queue */
        size_t message_size = eas_message_queue_pop(self.message_queue, (uint8_t *const)&message);
        /* Received a new message! */
        const Event *const generic_event = (const Event *const)message;
        switch (generic_event->id) {
        case EVENT_ID_INIT: {
            EAS_ASSERT(message_size == sizeof(Event));
            handle_init_event();
            break;
        }
        case EVENT_ID_NEW_TEMPERATURE_SAMPLE: {
            EAS_ASSERT(message_size == sizeof(NewTemperatureSampleEvent));
            const NewTemperatureSampleEvent *const event = (const NewTemperatureSampleEvent *const)message;
            handle_new_temperature_sample_event(event);
            break;
        }
        case EVENT_ID_NEW_PRESSURE_SAMPLE: {
            EAS_ASSERT(message_size == sizeof(NewPressureSampleEvent));
            const NewPressureSampleEvent *const event = (const NewPressureSampleEvent *const)message;
            handle_new_pressure_sample_event(event);
            break;
        }
        case EVENT_ID_NEW_HUMIDITY_SAMPLE: {
            EAS_ASSERT(message_size == sizeof(NewHumiditySampleEvent));
            const NewHumiditySampleEvent *const event = (const NewHumiditySampleEvent *const)message;
            handle_new_humidity_sample_event(event);
            break;
        }
        case EVENT_ID_NEW_LIGHT_INTENSITY_SAMPLE: {
            EAS_ASSERT(message_size == sizeof(NewLightIntensitySampleEvent));
            const NewLightIntensitySampleEvent *const event = (const NewLightIntensitySampleEvent *const)message;
            handle_new_light_intensity_sample_event(event);
            break;
        }
        case EVENT_ID_VOID_CB_WITH_USER_DATA: {
            EAS_ASSERT(message_size == sizeof(VoidCbWithUserDataEvent));
            const VoidCbWithUserDataEvent *const event = (const VoidCbWithUserDataEvent *const)message;
            handle_void_cb_with_user_data_event(event);
            break;
        }
        default:
            EAS_ASSERT(0); // Invalid event id
            break;
        }
    }
}

/**
 * @brief Push event to the event queue.
 *
 * @param event Event bytes. Should be a pointer the Event data type that corresponds to the event being pushed.
 * @param event_size Number of event bytes. Should be sizeof(<event_being_pushed>Event).
 */
static void push_event_to_queue(const uint8_t *const event, size_t event_size)
{
    /* Asserting because our system is designed in a way that there should always be space in the message queue. If it
     * got full, something went wrong. */
    EAS_ASSERT(eas_message_queue_push(self.message_queue, event, event_size));
}

void central_event_queue_init()
{
    self.message_queue =
        eas_message_queue_create(self.message_queue_buf, CONFIG_CENTRAL_EVENT_QUEUE_MESSAGE_QUEUE_BUF_SIZE,
                                 CENTRAL_EVENT_QUEUE_MAX_MESSAGE_SIZE);
    eas_thread_create(central_event_queue_thread_run);
}

void central_event_queue_submit_init_event()
{
    Event event = {
        .id = EVENT_ID_INIT,
    };
    push_event_to_queue((const uint8_t *const)&event, sizeof(Event));
}

void central_event_queue_submit_new_temperature_sample_event(Temperature temperature)
{
    NewTemperatureSampleEvent event = {
        .event.id = EVENT_ID_NEW_TEMPERATURE_SAMPLE,
        .sample = temperature,
    };
    push_event_to_queue((const uint8_t *const)&event, sizeof(NewTemperatureSampleEvent));
}

void central_event_queue_submit_new_pressure_sample_event(Pressure pressure)
{
    NewPressureSampleEvent event = {
        .event.id = EVENT_ID_NEW_PRESSURE_SAMPLE,
        .sample = pressure,
    };
    push_event_to_queue((const uint8_t *const)&event, sizeof(NewPressureSampleEvent));
}

void central_event_queue_submit_new_humidity_sample_event(Humidity humidity)
{
    NewHumiditySampleEvent event = {
        .event.id = EVENT_ID_NEW_HUMIDITY_SAMPLE,
        .sample = humidity,
    };
    push_event_to_queue((const uint8_t *const)&event, sizeof(NewHumiditySampleEvent));
}

void central_event_queue_submit_new_light_intensity_sample_event(LightIntensity light_intensity)
{
    NewLightIntensitySampleEvent event = {
        .event.id = EVENT_ID_NEW_LIGHT_INTENSITY_SAMPLE,
        .sample = light_intensity,
    };
    push_event_to_queue((const uint8_t *const)&event, sizeof(NewLightIntensitySampleEvent));
}

void central_event_queue_submit_void_cb_with_user_data_event(CentralEventQueueVoidCbWithUserData cb, void *user_data)
{
    EAS_ASSERT(cb);
    VoidCbWithUserDataEvent event = {
        .event.id = EVENT_ID_VOID_CB_WITH_USER_DATA,
        .cb = cb,
        .user_data = user_data,
    };
    push_event_to_queue((const uint8_t *const)&event, sizeof(VoidCbWithUserDataEvent));
}
