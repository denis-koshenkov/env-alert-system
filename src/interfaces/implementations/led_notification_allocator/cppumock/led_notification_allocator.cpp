#include "CppUTestExt/MockSupport.h"
#include "led_notification_allocator.h"

LedNotification *led_notification_allocator_alloc()
{
    mock().actualCall("led_notification_allocator_alloc");
    return (LedNotification *)mock().pointerReturnValue();
}

void led_notification_allocator_free(LedNotification *led_notification)
{
    mock().actualCall("led_notification_allocator_free").withParameter("led_notification", led_notification);
}
