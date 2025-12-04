#include "msg_transceiver.h"
#include "hal/transceiver.h"
#include "eas_assert.h"

#define MSG_TRANSCEIVER_MESSAGE_ID_ALERT_STATUS_CHANGE 0
#define MSG_TRANSCEIVER_MESSAGE_ID_REMOVE_ALERT 1
#define MSG_TRANSCEIVER_MESSAGE_ID_ADD_ALERT 2

static MsgTransceiverMessageSentCb message_sent_cb = NULL;
static MsgTransceiverRemoveAlertCb remove_alert_cb = NULL;
static void *remove_alert_cb_user_data = NULL;
static MsgTransceiverAddAlertCb add_alert_cb = NULL;

static void transmit_complete_cb(bool result, void *user_data)
{
    if (message_sent_cb) {
        message_sent_cb(result, user_data);
    }
}

/**
 * @brief Handle receiving a "remove alert" message.
 *
 * @param bytes Received bytes excluding the first message id byte.
 * @param num_bytes Number of bytes in the @p bytes array.
 */
static void handle_remove_alert_message(const uint8_t *const bytes, size_t num_bytes)
{
    EAS_ASSERT(bytes);

    /* There should be exactly one byte in the payload - alert id */
    if (num_bytes != 1) {
        return;
    }

    uint8_t alert_id = bytes[0];
    if (remove_alert_cb) {
        remove_alert_cb(alert_id, remove_alert_cb_user_data);
    }
}

/**
 * @brief Convert four bytes in little endian to an integer of type uint32_t.
 *
 * @param bytes The four bytes at this address are used for conversion.
 *
 * @return uint32_t Resulting integer.
 */
static uint32_t four_little_endian_bytes_to_uint32(const uint8_t *const bytes)
{
    return (((uint32_t)bytes[0]) | (((uint32_t)(bytes[1])) << 8) | (((uint32_t)(bytes[2])) << 16) |
            (((uint32_t)(bytes[3])) << 24));
}

/**
 * @brief Convert two bytes in little endian to an integer of type uint16_t.
 *
 * @param bytes The two bytes at this address are used for conversion.
 *
 * @return uint16_t Resulting integer.
 */
static uint16_t two_little_endian_bytes_to_uint16(const uint8_t *const bytes)
{
    return (((uint16_t)(bytes[0])) | (((uint16_t)(bytes[1])) << 8));
}

/**
 * @brief Check if a number of bytes is available in an array.
 *
 * There is a byte array of length @p total_num_bytes. @p index identifies a byte in the array - this is the next
 * available byte. All bytes at indices >= @p index and < @p total_num_bytes are considered available bytes. This
 * function checks whether this number of available bytes is greater than or equal to @p desired_num_available_bytes.
 *
 * @param desired_num_available_bytes The number of bytes that the caller wants to still be available in the array.
 * @param total_num_bytes Total number of bytes in the array.
 * @param index Index of the next available byte in the array.
 *
 * @return true There are at least @p desired_num_available_bytes bytes available in the array, or @p
 * desired_num_available_bytes is 0 - 0 bytes are always available.
 * @return false There are less than @p desired_num_available_bytes bytes available in the array, or @p index does not
 * point to a valid byte in the array, i.e. @p index >= @p total_num_bytes.
 */
static bool is_x_bytes_available(size_t desired_num_available_bytes, size_t total_num_bytes, size_t index)
{
    if (desired_num_available_bytes == 0) {
        /* 0 bytes are always available */
        return true;
    }
    if (index >= total_num_bytes) {
        /* 0 bytes available, already reached the end */
        return false;
    }
    size_t actual_num_available_bytes = total_num_bytes - index;
    return (actual_num_available_bytes >= desired_num_available_bytes);
}

static bool parse_alert_id(const uint8_t *const bytes, size_t num_bytes, size_t *const index, uint8_t *const alert_id)
{
    if (!is_x_bytes_available(1, num_bytes, *index)) {
        /* No bytes left */
        return false;
    }

    *alert_id = bytes[(*index)++];
    return true;
}

static bool parse_warmup_period(const uint8_t *const bytes, size_t num_bytes, size_t *const index,
                                uint32_t *const warmup_period)
{
    if (!is_x_bytes_available(4, num_bytes, *index)) {
        return false;
    }
    *warmup_period = four_little_endian_bytes_to_uint32(&bytes[*index]);
    *index += 4;
    return true;
}

static bool parse_cooldown_period(const uint8_t *const bytes, size_t num_bytes, size_t *const index,
                                  uint32_t *const cooldown_period)
{
    if (!is_x_bytes_available(4, num_bytes, *index)) {
        return false;
    }
    *cooldown_period = four_little_endian_bytes_to_uint32(&bytes[*index]);
    *index += 4;
    return true;
}

static bool parse_notification_type(const uint8_t *const bytes, size_t num_bytes, size_t *const index,
                                    NotificationType *const notification_type)
{
    if (!is_x_bytes_available(1, num_bytes, *index)) {
        return false;
    }
    /* Only bits 0 and 1 are allowed to be set */
    if (bytes[*index] > 0x3) {
        return false;
    }

    notification_type->connectivity = (bytes[*index] & ((uint8_t)0x1U)) ? 1 : 0;
    notification_type->led = (bytes[*index] & ((uint8_t)0x2U)) ? 1 : 0;
    (*index)++;
    return true;
}

static bool parse_led_color(const uint8_t *const bytes, size_t num_bytes, size_t *const index, uint8_t *const led_color)
{
    if (!is_x_bytes_available(1, num_bytes, *index)) {
        return false;
    }
    *led_color = bytes[(*index)++];
    return true;
}

static bool parse_led_pattern(const uint8_t *const bytes, size_t num_bytes, size_t *const index,
                              uint8_t *const led_pattern)
{
    if (!is_x_bytes_available(1, num_bytes, *index)) {
        return false;
    }
    *led_pattern = bytes[(*index)++];
    return true;
}

static bool parse_temperature_constraint_value(const uint8_t *const bytes, size_t num_bytes, size_t *const index,
                                               MsgTransceiverTemperature *const constraint_value)
{
    if (!is_x_bytes_available(2, num_bytes, *index)) {
        return false;
    }
    /* Store the two bytes in a variable */
    uint16_t value_unsigned = two_little_endian_bytes_to_uint16(&bytes[*index]);
    /* Interpret the two bytes as a two-byte signed integer */
    int16_t *value_signed_p = (int16_t *)&value_unsigned;
    /* Assign the two-byte signed integer as the temperature constraint value */
    *constraint_value = *value_signed_p;
    *index += 2;
    return true;
}

static bool parse_pressure_constraint_value(const uint8_t *const bytes, size_t num_bytes, size_t *const index,
                                            MsgTransceiverPressure *const constraint_value)
{
    if (!is_x_bytes_available(2, num_bytes, *index)) {
        return false;
    }
    *constraint_value = two_little_endian_bytes_to_uint16(&bytes[*index]);
    *index += 2;
    return true;
}

static bool parse_humidity_constraint_value(const uint8_t *const bytes, size_t num_bytes, size_t *const index,
                                            MsgTransceiverHumidity *const constraint_value)
{
    if (!is_x_bytes_available(2, num_bytes, *index)) {
        return false;
    }
    *constraint_value = two_little_endian_bytes_to_uint16(&bytes[*index]);
    *index += 2;
    return true;
}

static bool parse_light_intensity_constraint_value(const uint8_t *const bytes, size_t num_bytes, size_t *const index,
                                                   MsgTransceiverLightIntensity *const constraint_value)
{
    if (!is_x_bytes_available(4, num_bytes, *index)) {
        return false;
    }
    *constraint_value = four_little_endian_bytes_to_uint32(&bytes[*index]);
    *index += 4;
    return true;
}

static bool parse_variable_requirement(const uint8_t *const bytes, size_t num_bytes, size_t *const index,
                                       MsgTransceiverVariableRequirement *const requirement)
{
    if (!is_x_bytes_available(2, num_bytes, *index)) {
        return false;
    }
    requirement->variable_identifier = bytes[(*index)++];
    requirement->operator = bytes[(*index)++];
    switch (requirement->variable_identifier) {
    case MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_TEMPERATURE:
        if (!parse_temperature_constraint_value(bytes, num_bytes, index, &requirement->constraint_value.temperature)) {
            return false;
        }
        break;
    case MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_PRESSURE:
        if (!parse_pressure_constraint_value(bytes, num_bytes, index, &requirement->constraint_value.pressure)) {
            return false;
        }
        break;
    case MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_HUMIDITY:
        if (!parse_humidity_constraint_value(bytes, num_bytes, index, &requirement->constraint_value.humidity)) {
            return false;
        }
        break;
    case MSG_TRANSCEIVER_VARIABLE_IDENTIFIER_LIGHT_INTENSITY:
        if (!parse_light_intensity_constraint_value(bytes, num_bytes, index,
                                                    &requirement->constraint_value.light_intensity)) {
            return false;
        }
        break;
    default:
        break;
    }
    return true;
}

static bool parse_ored_requirement(const uint8_t *const bytes, size_t num_bytes, size_t *const index,
                                   MsgTransceiverAlertCondition *const alert_condition)
{
    if (!is_x_bytes_available(1, num_bytes, *index)) {
        return false;
    }
    size_t num_variable_requirements_in_ored_requirement = bytes[(*index)++];
    for (size_t i = 0; i < num_variable_requirements_in_ored_requirement; i++) {
        MsgTransceiverVariableRequirement *const requirement =
            &(alert_condition->variable_requirements[alert_condition->num_variable_requirements]);
        if (!parse_variable_requirement(bytes, num_bytes, index, requirement)) {
            return false;
        }
        requirement->is_last_in_ored_requirement = (i == (num_variable_requirements_in_ored_requirement - 1));
        alert_condition->num_variable_requirements++;
    }
    return true;
}

static bool parse_num_ored_requirements(const uint8_t *const bytes, size_t num_bytes, size_t *const index,
                                        uint8_t *const num_ored_requirements)
{
    if (!is_x_bytes_available(1, num_bytes, *index)) {
        return false;
    }
    *num_ored_requirements = bytes[(*index)++];
    return true;
}

/**
 * @brief Handle receiving a "add alert" message.
 *
 * @param bytes Received bytes excluding the first message id byte.
 * @param num_bytes Number of bytes in the @p bytes array.
 */
static void handle_add_alert_message(const uint8_t *const bytes, size_t num_bytes)
{
    MsgTransceiverAlert alert;
    size_t index = 0;
    if (!parse_alert_id(bytes, num_bytes, &index, &alert.alert_id)) {
        return;
    }
    if (!parse_warmup_period(bytes, num_bytes, &index, &alert.warmup_period)) {
        return;
    }
    if (!parse_cooldown_period(bytes, num_bytes, &index, &alert.cooldown_period)) {
        return;
    }
    if (!parse_notification_type(bytes, num_bytes, &index, &alert.notification_type)) {
        return;
    }
    if (alert.notification_type.led) {
        if (!parse_led_color(bytes, num_bytes, &index, &alert.led_color)) {
            return;
        }
        if (!parse_led_pattern(bytes, num_bytes, &index, &alert.led_pattern)) {
            return;
        }
    }

    uint8_t num_ored_requirements = 0;
    if (!parse_num_ored_requirements(bytes, num_bytes, &index, &num_ored_requirements)) {
        return;
    }

    /* parse_ored_requirement will increment this field whenever it adds a variable requirement to alert condition */
    alert.alert_condition.num_variable_requirements = 0;
    for (size_t i = 0; i < num_ored_requirements; i++) {
        if (!parse_ored_requirement(bytes, num_bytes, &index, &alert.alert_condition)) {
            return;
        }
    }

    if (add_alert_cb) {
        add_alert_cb(&alert, NULL);
    }
}

static void receive_cb(uint8_t *bytes, size_t num_bytes, void *user_data)
{
    EAS_ASSERT(bytes);
    if (num_bytes == 0) {
        return;
    }

    uint8_t message_id = bytes[0];
    switch (message_id) {
    case MSG_TRANSCEIVER_MESSAGE_ID_REMOVE_ALERT:
        handle_remove_alert_message(&bytes[1], num_bytes - 1);
        break;
    case MSG_TRANSCEIVER_MESSAGE_ID_ADD_ALERT:
        handle_add_alert_message(&bytes[1], num_bytes - 1);
        break;
    default:
        break;
    }
}

void msg_transceiver_init()
{
    transceiver_set_receive_cb(receive_cb, NULL);
}

void msg_transceiver_send_alert_status_change_message(uint8_t alert_id, bool is_raised, MsgTransceiverMessageSentCb cb,
                                                      void *user_data)
{
    message_sent_cb = cb;
    uint8_t bytes[3];
    bytes[0] = MSG_TRANSCEIVER_MESSAGE_ID_ALERT_STATUS_CHANGE;
    bytes[1] = alert_id;
    bytes[2] = is_raised ? 0x1 : 0x0;
    /* Pass user_data so that it is available inside transmit_complete_cb, and we can pass it as a parameter to
     * message_sent_cb */
    transceiver_transmit(bytes, 3, transmit_complete_cb, user_data);
}

void msg_transceiver_set_add_alert_cb(MsgTransceiverAddAlertCb cb, void *user_data)
{
    add_alert_cb = cb;
}

void msg_transceiver_set_remove_alert_cb(MsgTransceiverRemoveAlertCb cb, void *user_data)
{
    remove_alert_cb = cb;
    remove_alert_cb_user_data = user_data;
}

void msg_transceiver_deinit()
{
    transceiver_unset_receive_cb();
}
