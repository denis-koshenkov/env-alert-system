/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef BT_LBS_H_
#define BT_LBS_H_

/**@file
 * @defgroup bt_lbs LED Button Service API
 * @{
 * @brief API for the LED Button Service (LBS).
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include <zephyr/types.h>

/* STEP 1 - Define the 128 bit UUIDs for the GATT service and its characteristics in */
/** @brief EASS Service UUID. */
#define BT_UUID_EASS_VAL BT_UUID_128_ENCODE(0x7CEC6BFA, 0x6B62, 0x4371, 0x99EB, 0xA335315223C5)

/** @brief TX Characteristic UUID. */
#define BT_UUID_EASS_TX_VAL BT_UUID_128_ENCODE(0x7CEC6BFB, 0x6B62, 0x4371, 0x99EB, 0xA335315223C5)

/** @brief RX Characteristic UUID. */
#define BT_UUID_EASS_RX_VAL BT_UUID_128_ENCODE(0x7CEC6BFC, 0x6B62, 0x4371, 0x99EB, 0xA335315223C5)

#define BT_UUID_EASS BT_UUID_DECLARE_128(BT_UUID_EASS_VAL)
#define BT_UUID_EASS_TX BT_UUID_DECLARE_128(BT_UUID_EASS_TX_VAL)
#define BT_UUID_EASS_RX BT_UUID_DECLARE_128(BT_UUID_EASS_RX_VAL)

/** @brief Callback type for when an LED state change is received. */
typedef void (*led_cb_t)(const bool led_state);

/** @brief Callback type for when the button state is pulled. */
typedef bool (*button_cb_t)(void);

/** @brief Callback struct used by the LBS Service. */
struct my_lbs_cb {
    /** LED state change callback. */
    led_cb_t led_cb;
    /** Button read callback. */
    button_cb_t button_cb;
};

/** @brief Initialize the LBS Service.
 *
 * This function registers application callback functions with the My LBS
 * Service
 *
 * @param[in] callbacks Struct containing pointers to callback functions
 *			used by the service. This pointer can be NULL
 *			if no callback functions are defined.
 *
 *
 * @retval 0 If the operation was successful.
 *           Otherwise, a (negative) error code is returned.
 */
int my_lbs_init(struct my_lbs_cb *callbacks);

void my_lbs_send_button_state_indicate(bool button_state);

void my_lbs_send_sensor_notify(uint32_t sensor_value);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* BT_LBS_H_ */
