/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>

#include "eass.h"
#include "eas_log.h"

EAS_LOG_ENABLE_IN_FILE();

static EassCbs eass_cbs = {
    .received = NULL,
    .send_enabled = NULL,
};

static void eass_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    bool enabled = (value == BT_GATT_CCC_NOTIFY);
    EAS_LOG_INF("EASS Notification has been turned %s", enabled ? "on" : "off");
    if (eass_cbs.send_enabled) {
        eass_cbs.send_enabled(enabled);
    }
}

static ssize_t on_receive(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len,
                          uint16_t offset, uint8_t flags)
{
    if (eass_cbs.received) {
        eass_cbs.received(buf, len);
    }
    return len;
}

// clang-format off
BT_GATT_SERVICE_DEFINE(
    eass_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_EASS),
	BT_GATT_CHARACTERISTIC(BT_UUID_EASS_TX, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, NULL, NULL, NULL),
	BT_GATT_CCC(eass_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CHARACTERISTIC(BT_UUID_EASS_RX, BT_GATT_CHRC_WRITE | BT_GATT_CHRC_WRITE_WITHOUT_RESP, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, NULL, on_receive, NULL),
);
// clang-format on

void eass_init(const EassCbs *const cbs)
{
    if (cbs) {
        eass_cbs.received = cbs->received;
        eass_cbs.send_enabled = cbs->send_enabled;
    }
}

bool eass_send(const uint8_t *data, uint16_t len, bt_gatt_complete_func_t cb, void *user_data)
{
    struct bt_gatt_notify_params notify_params = {
        .uuid = NULL,
        .attr = &eass_svc.attrs[2],
        .data = data,
        .len = len,
        .func = cb,
        .user_data = user_data,
    };
    int ret = bt_gatt_notify_cb(NULL, &notify_params);
    return (ret == 0);
}
