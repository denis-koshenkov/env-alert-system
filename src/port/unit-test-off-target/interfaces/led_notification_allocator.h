#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_LED_NOTIFICATION_ALLOCATOR_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_LED_NOTIFICATION_ALLOCATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "led_manager_private.h"

LedNotification *led_notification_allocator_alloc();

void led_notification_allocator_free(LedNotification *led_notification);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INTERFACES_LED_NOTIFICATION_ALLOCATOR_H */
