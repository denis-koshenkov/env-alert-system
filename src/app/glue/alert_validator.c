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

bool alert_validator_is_valid_alert(const MsgTransceiverAlert *alert)
{
    return is_valid_led_color(alert->led_color);
}
