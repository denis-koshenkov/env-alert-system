#include <stddef.h>
#include <stdbool.h>

#include "led_manager_private.h"
#include "led_manager.h"
#include "hal/hw_platform.h"
#include "eas_timer.h"
#include "utils/linked_list.h"
#include "config.h"
#include "led_notification_allocator.h"
#include "eas_assert.h"

#ifndef CONFIG_LED_MANAGER_NOTIFICATION_DURATION_SECONDS
#define CONFIG_LED_MANAGER_NOTIFICATION_DURATION_SECONDS 5
#endif

#define LED_MANAGER_NOTIFICATION_DURATION_MS (CONFIG_LED_MANAGER_NOTIFICATION_DURATION_SECONDS * 1000)

static size_t num_notifications = 0;
static LedNotification *displayed_notification;
static void *iterator = NULL;

static LinkedList get_linked_list_instance()
{
    static LinkedList instance;
    static bool is_created = false;
    if (!is_created) {
        instance = linked_list_create();
        is_created = true;
    }
    return instance;
}

/**
 * @brief Sets displayed_notification to the next notification.
 *
 * @pre Linked list iterator has been initialized.
 */
static void advance_notification_iterator()
{
    bool element_valid = linked_list_iterator_next(&iterator, (void **)&displayed_notification);
    if (!element_valid) {
        iterator = linked_list_iterator_init(get_linked_list_instance());
        element_valid = linked_list_iterator_next(&iterator, (void **)&displayed_notification);
        EAS_ASSERT(element_valid);
    }
}

static void display_next_notification()
{
    advance_notification_iterator();
    hw_platform_get_led()->set(displayed_notification->led_color, displayed_notification->led_pattern);
}

static void notification_timer_cb(void *user_data)
{
    display_next_notification();
}

static EasTimer get_timer_instance()
{
    static EasTimer instance;
    static bool is_created = false;
    if (!is_created) {
        instance =
            eas_timer_create(LED_MANAGER_NOTIFICATION_DURATION_MS, EAS_TIMER_PERIODIC, notification_timer_cb, NULL);
        is_created = true;
    }
    return instance;
}

static void add_notification_to_list(LedNotification *led_notification)
{
    linked_list_append(get_linked_list_instance(), led_notification);
    num_notifications++;
}

static bool remove_condition_cb(void *element, void *user_data)
{
    EAS_ASSERT(element);
    EAS_ASSERT(user_data);
    LedNotification *current_notif = (LedNotification *)element;
    LedNotification *notif_to_remove = (LedNotification *)user_data;

    bool should_remove = ((current_notif->led_color == notif_to_remove->led_color) &&
                          (current_notif->led_pattern == notif_to_remove->led_pattern));
    return should_remove;
}

static void pre_remove_cb(void *element, void *user_data)
{
    EAS_ASSERT(element);
    led_notification_allocator_free((LedNotification *)element);
}

static void remove_notification_from_list(LedColor led_color, LedPattern led_pattern)
{
    LedNotification led_notification_to_remove = {.led_color = led_color, .led_pattern = led_pattern};

    /* Remove at most one element. It can be the case that there are several notification with the same color and
     * pattern. Here we want to remove only the first occurrence. Free the allocated led notification memory before
     * removing it from the list. */
    size_t num_removed = linked_list_remove_on_condition_with_limit(get_linked_list_instance(), 1, remove_condition_cb,
                                                                    &led_notification_to_remove, pre_remove_cb, NULL);
    num_notifications--;
}

static void start_switching_notification_display()
{
    iterator = linked_list_iterator_init(get_linked_list_instance());
    /* This function is called when there is one notification currently in the list, and the second one has just been
     * added. The first notification is already being displayed. We advance the iterator to set displayed_notification
     * to the first notification, but we do not set the led to it, because it is already set to the first notification.
     * When the timer expires, the iterator will advance to the second notification, and the led will switch to the
     * second notification. */
    advance_notification_iterator();
    eas_timer_start(get_timer_instance());
}

void led_manager_add_notification(LedColor led_color, LedPattern led_pattern)
{
    LedNotification *led_notification = led_notification_allocator_alloc();
    led_notification->led_color = led_color;
    led_notification->led_pattern = led_pattern;
    add_notification_to_list(led_notification);

    if (num_notifications == 1) {
        /* This is the only notification - set the led immediately */
        hw_platform_get_led()->set(led_notification->led_color, led_notification->led_pattern);
    } else if (num_notifications == 2) {
        /* This is the second notification. Need to start alternating between displaying the two notifications. */
        start_switching_notification_display();
    }
}

void led_manager_remove_notification(LedColor led_color, LedPattern led_pattern)
{
    remove_notification_from_list(led_color, led_pattern);
    if (num_notifications == 0) {
        /* The last notification was removed. Turn off the led - no notifications to display. */
        hw_platform_get_led()->turn_off();
    } else if (num_notifications == 1) {
        /* There is now one notification left, we do not need to alternate between the notifications - stop the
         * notification timer. */
        eas_timer_stop(get_timer_instance());
    } else {
        /* 2 or more notification left. We will continue using the linked list iterator to switch between notifications.
         * Since we removed an element from the list, we need to re-initialize the iterator, according to linked list
         * docs. */
        iterator = linked_list_iterator_init(get_linked_list_instance());

        /* We need to continue displaying notifications as if the removal did not happen. Find the currently displayed
         * notification in the list using the iterator, so that the next time the timer expires, we switch to displaying
         * the next notification. */
        LedNotification *current_notification = NULL;
        bool element_valid;
        do {
            element_valid = linked_list_iterator_next(&iterator, (void **)&current_notification);
        } while ((current_notification != displayed_notification) && element_valid);
        /* If element is invalid here, this means that we reached the end of the list and did not find the currently
         * displayed notification. This should never happen. */
        EAS_ASSERT(element_valid);
    }
}
