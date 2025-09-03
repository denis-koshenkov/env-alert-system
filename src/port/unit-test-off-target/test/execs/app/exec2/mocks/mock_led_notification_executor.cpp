#include "CppUTestExt/MockSupport.h"
#include "mock_led_notification_executor.h"

void led_notification_executor_execute(LedColor led_color, LedPattern led_pattern, bool should_be_displayed)
{
    mock()
        .actualCall("led_notification_executor_execute")
        .withParameter("led_color", led_color)
        .withParameter("led_pattern", led_pattern)
        .withParameter("should_be_displayed", should_be_displayed);
}
