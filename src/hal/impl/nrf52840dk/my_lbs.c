/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 *  @brief LED Button Service (LBS) sample
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "my_lbs.h"

#include <zephyr/logging/log.h>

#include "eas_log.h"

EAS_LOG_ENABLE_IN_FILE();

static bool button_state;
static struct my_lbs_cb lbs_cb;

static bool indicate_enabled = false;
static bool notify_mysensor_enabled = false;

static struct bt_gatt_indicate_params ind_params;

/* Configuration change callback function */
static void mylbsbc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    indicate_enabled = (value == BT_GATT_CCC_INDICATE);
    EAS_LOG_INF("%s", indicate_enabled ? "Button indicate enabled" : "Button indicate disabled");
}

static void mylbsbc_ccc_mysensor_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    notify_mysensor_enabled = (value == BT_GATT_CCC_NOTIFY);
}

static void nus_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    EAS_LOG_INF("EASS Notification has been turned %s", value == BT_GATT_CCC_NOTIFY ? "on" : "off");
    if (eass_cb.send_state_changed) {
        eass_cb.send_state_changed(value == BT_GATT_CCC_NOTIFY ? true : false);
    }
}

// This function is called when a remote device has acknowledged the indication at its host layer
static void indicate_cb(struct bt_conn *conn, struct bt_gatt_indicate_params *params, uint8_t err)
{
    EAS_LOG_INF("Indication %s", err != 0U ? "fail" : "success");
}

/* STEP 6 - Implement the write callback function of the LED characteristic */
static ssize_t write_led(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len,
                         uint16_t offset, uint8_t flags)
{
    LOG_DBG("Attribute write, handle: %u, conn: %p", attr->handle, (void *)conn);

    if (len != 1U) {
        LOG_DBG("Write led: Incorrect data length");
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
    }

    if (offset != 0) {
        LOG_DBG("Write led: Incorrect data offset");
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
    }

    if (lbs_cb.led_cb) {
        // Read the received value
        uint8_t val = *((uint8_t *)buf);

        if (val == 0x00 || val == 0x01) {
            // Call the application callback function to update the LED state
            lbs_cb.led_cb(val ? true : false);
        } else {
            LOG_DBG("Write led: Incorrect value");
            return BT_GATT_ERR(BT_ATT_ERR_VALUE_NOT_ALLOWED);
        }
    }

    return len;
}

/* STEP 5 - Implement the read callback function of the Button characteristic */
static ssize_t read_button(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len,
                           uint16_t offset)
{
    // get a pointer to button_state which is passed in the BT_GATT_CHARACTERISTIC() and stored in attr->user_data
    const char *value = attr->user_data;

    LOG_DBG("Attribute read, handle: %u, conn: %p", attr->handle, (void *)conn);

    if (lbs_cb.button_cb) {
        // Call the application callback function to update the get the current value of the button
        button_state = lbs_cb.button_cb();
        return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(*value));
    }

    return 0;
}

// clang-format off
/* LED Button Service Declaration */
BT_GATT_SERVICE_DEFINE(
    my_lbs_svc, BT_GATT_PRIMARY_SERVICE(BT_UUID_LBS),
    /* Button characteristic */
    BT_GATT_CHARACTERISTIC(BT_UUID_LBS_BUTTON, BT_GATT_CHRC_READ | BT_GATT_CHRC_INDICATE, BT_GATT_PERM_READ, read_button, NULL, &button_state),
    /* Client Characteristic Configuration Descriptor */
	BT_GATT_CCC(mylbsbc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    /* STEP 4 - Create and add the LED characteristic. */
    BT_GATT_CHARACTERISTIC(BT_UUID_LBS_LED, BT_GATT_CHRC_WRITE, BT_GATT_PERM_WRITE, NULL, write_led, NULL),
    /* MYSENSOR characteristic */
    BT_GATT_CHARACTERISTIC(BT_UUID_LBS_MYSENSOR, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_NONE, NULL,
                NULL, NULL),
	BT_GATT_CCC(mylbsbc_ccc_mysensor_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);
// clang-format on

// clang-format off
BT_GATT_SERVICE_DEFINE(
    eass_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_EASS),
	BT_GATT_CHARACTERISTIC(BT_UUID_EASS_TX, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, NULL, NULL, NULL),
	BT_GATT_CCC(eass_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CHARACTERISTIC(BT_UUID_EASS_RX, BT_GATT_CHRC_WRITE | BT_GATT_CHRC_WRITE_WITHOUT_RESP, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, NULL, on_receive, NULL),
);
// clang-format on

void my_lbs_init(struct my_lbs_cb *callbacks)
{
    if (callbacks) {
        lbs_cb.led_cb = callbacks->led_cb;
        lbs_cb.button_cb = callbacks->button_cb;
    }

    return 0;
}

void my_lbs_send_button_state_indicate(bool button_state)
{
    if (!indicate_enabled) {
        EAS_LOG_INF("Button state indication failed: indicate disabled");
        return;
    }
    ind_params.attr = &my_lbs_svc.attrs[2];
    ind_params.func = indicate_cb; // A remote device has ACKed at its host layer (ATT ACK)
    ind_params.destroy = NULL;
    ind_params.data = &button_state;
    ind_params.len = sizeof(button_state);
    int err = bt_gatt_indicate(NULL, &ind_params);
    if (err) {
        EAS_LOG_INF("Failed to indicate button state");
    }
}

void my_lbs_send_sensor_notify(uint32_t sensor_value)
{
    if (!notify_mysensor_enabled) {
        EAS_LOG_INF("Mysensor notification failed: notify disabled");
    }

    int err = bt_gatt_notify(NULL, &my_lbs_svc.attrs[7], &sensor_value, sizeof(sensor_value));
    if (err) {
        EAS_LOG_INF("Failed to notify mysensor");
    }
}
