#ifndef ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_LED_NOTIFICATION_ALLOCATOR_H
#define ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_LED_NOTIFICATION_ALLOCATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "led_manager_private.h"

/**
 * @brief Allocate memory for a led notification.
 *
 * @return LedNotification* If successful, points to allocated memory for led notification. If failed due to being out
 * of memory, returns NULL.
 */
LedNotification *led_notification_allocator_alloc();

/**
 * @brief Free a previously allocated led notification.
 *
 * @param led_notification Led notification buffer previously returned by @ref led_notification_allocator_alloc.
 */
void led_notification_allocator_free(LedNotification *led_notification);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_LED_NOTIFICATION_ALLOCATOR_H */
