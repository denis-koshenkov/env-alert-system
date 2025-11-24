#include "alert_validator.h"

/**
 * @brief Check whether LED color of alert notification is valid.
 *
 * @param led_color Led color.
 *
 * @return true @p led_color is one of the valid values from @ref MsgTransceiverLedColor.
 * @return false @p led_color is not a valid value from @ref MsgTransceiverLedColor.
 */
static bool is_valid_led_color(uint8_t led_color)
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
static bool is_valid_led_pattern(uint8_t led_pattern)
{
    return (led_pattern < MSG_TRANSCEIVER_LED_PATTERN_INVALID);
}

bool alert_validator_is_valid_alert(const MsgTransceiverAlert *alert)
{
    return is_valid_led_color(alert->led_color) && is_valid_led_pattern(alert->led_pattern);
}
