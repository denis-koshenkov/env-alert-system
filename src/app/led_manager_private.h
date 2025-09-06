#ifndef ENV_ALERT_SYSTEM_SRC_APP_LED_MANAGER_PRIVATE_H
#define ENV_ALERT_SYSTEM_SRC_APP_LED_MANAGER_PRIVATE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/* Defined in a separate header so that led_notification_allocator module can include this header and know the size of
 * the led notification that it needs to allocate at compile time. */
typedef struct LedNotification {
    /**< Defined as uint8_t to save memory, use values from enum LedColor. */
    uint8_t led_color;
    /**< Defined as uint8_t to save memory, use values from enum LedPattern. */
    uint8_t led_pattern;
} LedNotification;

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_LED_MANAGER_PRIVATE_H */
