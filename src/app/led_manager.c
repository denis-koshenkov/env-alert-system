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
/** Points to the LedNotification that is currently being displayed. Should always point to a LedNotification element
 * currently in the linked list of notifications. NULL if the LED is turned off. */
static const LedNotification *displayed_notification = NULL;
static void *iterator = NULL;
static const LedNotification *removed_notification = NULL;
static const LedNotification *next_notification_after_removed = NULL;

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
 * @brief Reset notification iterator.
 *
 * After a call to this function, the iteration over the notification linked list will start from the beginning of the
 * list. The next call to @ref advance_notification_iterator will return the first notification in the notification
 * linked list.
 */
static void reset_notification_iterator()
{
    iterator = linked_list_iterator_init(get_linked_list_instance());
}

/**
 * @brief Advances notification list iterator to point to the next notification.
 *
 * If before calling this function the iterator is pointing to the last notification in the list, this function will
 * wrap around and return the first notifiction in the list.
 *
 * If there is only one notification in the list, this function will always return that one notification.
 *
 * @pre Linked list iterator has been reset by calling @ref reset_notification_iterator.
 * @pre There is at least one element present in the linked list.
 *
 * @return LedNotification * The notification that the now advanced iterator points to.
 */
static LedNotification *advance_notification_iterator()
{
    LedNotification *next_notification = NULL;
    bool element_valid = linked_list_iterator_next(&iterator, (void **)&next_notification);
    if (!element_valid) {
        iterator = linked_list_iterator_init(get_linked_list_instance());
        element_valid = linked_list_iterator_next(&iterator, (void **)&next_notification);
        EAS_ASSERT(element_valid);
    }
    return next_notification;
}

/**
 * @brief Reset the iterator and iterate until the requested notification.
 *
 * After a call to this function, a subsequent call to @ref advance_notification_iterator will return the next
 * notification in the list after @p led_notification.
 *
 * @param led_notification Led notification to iterate until.
 *
 * @warning Fires an assert if @p led_notification is not present in the notification list. This function is only
 * allowed to be called if the caller is sure that @p led_notification is present in the list.
 */
static void reset_iterator_and_iterate_until(const LedNotification *led_notification)
{
    EAS_ASSERT(led_notification);

    reset_notification_iterator();
    LedNotification *current_notification = NULL;
    bool element_valid;
    do {
        element_valid = linked_list_iterator_next(&iterator, (void **)&current_notification);
    } while ((current_notification != led_notification) && element_valid);
    /* If element is invalid here, this means that we reached the end of the list and did not find the requested
     * notification. This should never happen. */
    EAS_ASSERT(element_valid);
}

/**
 * @brief Set the led to display the color and pattern of the notification.
 *
 * @param led_notification Notification to display.
 *
 * @warning @p led_notification must always point to a led notification element that is currently present in the linked
 * list of notifications. It must not point to any stack-allocated LedNotifications.
 */
static void display_notification(const LedNotification *led_notification)
{
    displayed_notification = led_notification;
    hw_platform_get_led()->set(led_notification->led_color, led_notification->led_pattern);
}

/**
 * @brief Turn off the led.
 */
static void turn_off_led()
{
    displayed_notification = NULL;
    hw_platform_get_led()->turn_off();
}

static void display_next_notification()
{
    LedNotification *notification_to_display = advance_notification_iterator();
    display_notification(notification_to_display);
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

    const LedNotification *current_notif = (const LedNotification *)element;
    const LedNotification *notif_to_remove = (const LedNotification *)user_data;

    bool should_remove = ((current_notif->led_color == notif_to_remove->led_color) &&
                          (current_notif->led_pattern == notif_to_remove->led_pattern));
    return should_remove;
}

/**
 * @brief Frees the memory of the LedNotification list element.
 *
 * This function is passed as a callback to the remove function of the linked list to be executed before the element is
 * removed from the list.
 *
 * @param element Linked list element that is about to be removed from the list.
 * @param user_data Callback user data.
 */
static void free_led_notification(void *element, void *user_data)
{
    EAS_ASSERT(element);

    led_notification_allocator_free((LedNotification *)element);
}

/**
 * @brief Iterates through the list and saves the notification to be removed and the one after it.
 *
 * This function is passed as a callback to the remove function of the linked list to be executed before the element is
 * removed from the list.
 *
 * @param element Linked list element that is about to be removed from the list.
 * @param user_data Callback user data.
 *
 * @warning This function should only be passed as a callback to the remove function from the linked list module if
 * there are at least 2 elements present in the list. This is because the purpose of this function is to save two
 * notifications - the one that is being removed and the one that is in the list right after it.
 */
static void find_next_notification_after_removed_and_free(void *element, void *user_data)
{
    EAS_ASSERT(element);

    LedNotification *led_notification_to_remove = (LedNotification *)element;
    /* Save the notification to be removed, so that we can later check if we removed the notification that was being
     * displayed. */
    removed_notification = led_notification_to_remove;

    /* We need to find the next notification in the list after the removed one. Iterate until the notification
     * that is about to be removed. */
    reset_iterator_and_iterate_until(led_notification_to_remove);

    /* Advance the iterator once more to get the next notification after the one to be removed. */
    next_notification_after_removed = advance_notification_iterator();

    /* This assert will fire if there is only one element in the list. This is not allowed, since we want to save the
     * notification to be removed and the one right after it in the list. */
    EAS_ASSERT(led_notification_to_remove != next_notification_after_removed);

    /* Free the memory of the notification about to be removed. */
    led_notification_allocator_free(led_notification_to_remove);
}

static void remove_notification_from_list(LedColor led_color, LedPattern led_pattern,
                                          LinkedListPreRemoveCb pre_remove_cb)
{
    LedNotification led_notification_to_remove = {.led_color = led_color, .led_pattern = led_pattern};

    /* Remove at most one element. It can be the case that there are several notification with the same color and
     * pattern. Here we want to remove only the first occurrence. Free the allocated led notification memory before
     * removing it from the list. */
    size_t num_removed = linked_list_remove_on_condition_with_limit(get_linked_list_instance(), 1, remove_condition_cb,
                                                                    &led_notification_to_remove, pre_remove_cb, NULL);
    num_notifications--;
}

void led_manager_add_notification(LedColor led_color, LedPattern led_pattern)
{
    LedNotification *led_notification = led_notification_allocator_alloc();
    led_notification->led_color = led_color;
    led_notification->led_pattern = led_pattern;
    add_notification_to_list(led_notification);
    /* To ensure the if below covers all possibilities */
    EAS_ASSERT(num_notifications > 0);

    if (num_notifications == 1) {
        /* This is the only notification - set the led immediately */
        display_notification(led_notification);
    } else if (num_notifications == 2) {
        /* This is the second notification. Need to start alternating between displaying the two notifications. */
        reset_notification_iterator();
        /* We advance the iterator to point to the first notification, but we do not display it, because it is
         * already being displayed. When the timer expires, the iterator will advance to the second notification, and
         * the led will display the second notification. */
        /* Ignore return value - we only need to advance the iterator */
        advance_notification_iterator();
        eas_timer_start(get_timer_instance());
    } else {
        /* There were already 2 or more notifications in the list before this one was added. The switching between
         * different notifications was already ongoing. Since we added an element to the linked list of notifications,
         * we need to reset the iterator, as per linked list docs. Set the iterator to point to the currently displayed
         * notification - exactly as it was before adding this notification. */
        reset_iterator_and_iterate_until(displayed_notification);
    }
}

void led_manager_remove_notification(LedColor led_color, LedPattern led_pattern)
{
    if (num_notifications == 1) {
        remove_notification_from_list(led_color, led_pattern, free_led_notification);
        /* The last notification was removed. Turn off the led - no notifications to display. */
        turn_off_led();
    } else if (num_notifications == 2) {
        remove_notification_from_list(led_color, led_pattern, find_next_notification_after_removed_and_free);
        /* There were 2 notifications in the list, and now there is only one. If we were already displaying the
         * notification that is left, we do not need to do anything. However, if we were displaying the notification we
         * just removed, we need to set the led to the notification that is left. */
        if (displayed_notification != next_notification_after_removed) {
            display_notification(next_notification_after_removed);
        }

        /* Since there is only one notification left, we need to stop switching between notification. */
        eas_timer_stop(get_timer_instance());
    } else {
        remove_notification_from_list(led_color, led_pattern, find_next_notification_after_removed_and_free);
        if (displayed_notification == removed_notification) {
            /* We removed the notification that was being displayed. We need to start displaying the notification after
             * the one that was removed. */
            reset_iterator_and_iterate_until(next_notification_after_removed);
            display_notification(next_notification_after_removed);
            /* Restart the timer so that we display the notification for the full period. */
            eas_timer_start(get_timer_instance());
        } else {
            /* We removed a notification that was not being displayed. Since we removed an element from the list, we
             * need to reset the iterator and iterate until the currently displayed notification. This way, the next
             * time the timer expires, we switch to displaying the next notification. */
            reset_iterator_and_iterate_until(displayed_notification);
        }
    }
}
