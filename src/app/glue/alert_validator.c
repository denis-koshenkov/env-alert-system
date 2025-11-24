#include "alert_validator.h"

/**
 * @brief Check whether LED color of alert notification is valid.
 *
 * @param led_color Led color.
 *
 * @return true @p led_color is one of the valid values from @ref MsgTransceiverLedColor.
 * @return false @p led_color is not a valid value from @ref MsgTransceiverLedColor.
 */
static bool is_led_color_valid(uint8_t led_color)
{
    return (led_color < MSG_TRANSCEIVER_LED_COLOR_INVALID);
}

/**
 * @brief Check whether LED pattern of alert notification is valid.
 *
 * @param led_pattern Led pattern.
 *
 * @return true @p led_pattern is one of the valid values from @ref MsgTransceiverLedPattern.
 * @return false @p led_pattern is not a valid value from @ref MsgTransceiverLedPattern.
 */
static bool is_led_pattern_valid(uint8_t led_pattern)
{
    return (led_pattern < MSG_TRANSCEIVER_LED_PATTERN_INVALID);
}

/**
 * @brief Check whether notification type of alert is valid.
 *
 * @param notification_type Notification type.
 *
 * @return true Notification type is valid.
 * @return false Notification type is invalid.
 */
static bool is_notification_type_valid(NotificationType notification_type)
{
    /* At least one notification type should be enabled, otherwise there is no point in adding this alert - nothing will
     * happen when it gets raised. */
    return !((notification_type.connectivity == 0) && (notification_type.led == 0));
}

bool alert_validator_is_alert_valid(const MsgTransceiverAlert *alert)
{
    // clang-format off
    return (
        is_led_color_valid(alert->led_color)
        && is_led_pattern_valid(alert->led_pattern)
        && is_notification_type_valid(alert->notification_type)
    );
    // clang-format on
}
