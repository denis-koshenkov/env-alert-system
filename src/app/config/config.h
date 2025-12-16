#ifndef ENV_ALERT_SYSTEM_SRC_APP_CONFIG_CONFIG_H
#define ENV_ALERT_SYSTEM_SRC_APP_CONFIG_CONFIG_H

/**
 * @brief List of all configs that the application relies on.
 *
 * This file should not be included as a part of any build. The purpose of this file is to list all the configs that
 * should be defined by different ports.
 *
 * Each port should itself define a config.h file, add its location to the include path so that application modules can
 * find it, and define all of these configs in that file. It can also define additional configs for port-specific
 * interfaces.
 */

/** Should correspond to the number of times <module_name>_create() will be called in the program. */
#define CONFIG_VALUE_HOLDER_MAX_NUM_INSTANCES
#define CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES
#define CONFIG_PRESSURE_VALUE_MAX_NUM_INSTANCES
#define CONFIG_HUMIDITY_VALUE_MAX_NUM_INSTANCES
#define CONFIG_LIGHT_INTENSITY_VALUE_MAX_NUM_INSTANCES
#define CONFIG_LINKED_LIST_MAX_NUM_INSTANCES
#define CONFIG_VARIABLE_REQUIREMENT_LIST_MAX_NUM_INSTANCES
#define CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES
#define CONFIG_ALERT_RAISER_MAX_NUM_INSTANCES

/** Maximal size of one variable requirement in bytes. We have different variable requirement types - e.g.
 * TemperatureVariableRequirement, PressureVariableRequirement, and so on. Each of these types might have a different
 * size, because the sizes of data types such as Temperature and Pressure can be different. This config defines the size
 * of the largest variable requirement type.
 *
 * The size of each variable requirement type should be less than or equal to this config. It is the responsibility of
 * each variable requirement module to assert that.
 */
#define CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE

/** Maximal allowed number of variable requirements in one alert condition. */
#define CONFIG_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION

/** Maximal allowed number of variable requirements in one alert condition - config for the alert_condition module. Most
 * likely, should be set to CONFIG_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION. */
#define CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS

/** Message transceiver converts "add alert" message payload to structured data. This defines how many variable
 * conditions can be stored in the alert condition of the alert to be added. Most likely, should be set to
 * CONFIG_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION. */
#define CONFIG_MSG_TRANSCEIVER_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION

/** Maximal number of alerts that can be registered in the system at the same time. */
#define CONFIG_MAX_NUM_ALERTS

/** Defines how many variable requirements the system should be able to store simultaneously. Should be set to the
 * maximal number of allowed alerts times the maximal number of variable requirement per alert, i.e.
 * (CONFIG_MAX_NUM_ALERTS * CONFIG_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION). */
#define CONFIG_MAX_TOTAL_NUM_VARIABLE_REQUIREMENTS

/** Maximum number of variable requirements that can be simultaneously allocated by the variable requirement allocator.
 * Should be set to CONFIG_MAX_TOTAL_NUM_VARIABLE_REQUIREMENTS. */
#define CONFIG_VARIABLE_REQUIREMENT_ALLOCATOR_NUM_REQUIREMENTS

/** Defines how many AlertCondition instances the AlertConditions module creates. Set to CONFIG_MAX_NUM_ALERTS. */
#define CONFIG_ALERT_CONDITIONS_NUM_INSTANCES_TO_CREATE

/** Defines how many AlertRaiser instances the AlertRaisers module creates. Set to CONFIG_MAX_NUM_ALERTS. */
#define CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE

/** Defines the highest valid alert ID. Set to (CONFIG_MAX_NUM_ALERTS - 1). */
#define CONFIG_ALERT_VALIDATOR_MAX_ALLOWED_ALERT_ID

/**
 * @brief Defines the valid alert ids for which a connectivity notification can be sent.
 *
 * The valid alert ids are from 0 to CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS - 1, both including. Set to
 * CONFIG_MAX_NUM_ALERTS.
 */
#define CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS

/**
 * @brief Defines the valid alert ids for which a led notification can be sent.
 *
 * The valid alert ids are from 0 to CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS - 1, both including. Set to
 * CONFIG_MAX_NUM_ALERTS.
 */
#define CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS

/** If there is more than one led notification added to the led manager, the led manager periodically switches between
 * displaying all added notifications.
 *
 * This config defines for how many seconds one led notification is displayed before switching to displaying the next
 * notification. If there are 0 or 1 notifications added to the LED manager, the value of this config does not affect
 * the behavior.
 */
#define CONFIG_LED_MANAGER_NOTIFICATION_DURATION_SECONDS

/**
 * @brief Influences the period of time for which timer expiry callbacks are ignored after timer is started.
 *
 * Led manager uses a timer to switch between displaying notifications. In some cases, led manager needs to stop this
 * timer.
 *
 * After the timer is stopped, it is possible that its expiration callback is still executed. Led manager needs to
 * ignore that callback and not perform any actions, since that callback is from a timer that was already stopped.
 *
 * In order to achieve this, led manager ignores all timer callbacks that are executed before the expected expiry time
 * of the last started timer. For example, a timer is started for 5 seconds. If a timer expiry callback is executed 100
 * ms after the timer was started, that callback should be ignored, since we know for sure that this is not the expiry
 * callback of the currently running timer.
 *
 * However, it is not wise to ignore all expiry callbacks that are executed before exactly 5 seconds pass. Depending on
 * the timer implementation, it is possible that the timer expiry callback will be executed slightly earlier than the
 * expected time.
 *
 * This config defines how much earlier timer callbacks are allowed to be executed compared to their expected time.
 *
 * For example, a 5 second timer is started when current time is 1000 ms. If this margin is 0, then timer expiry
 * callbacks that are executed at time 6000 or later are accepted, all the ones that arrive earlier are ignored. If this
 * margin is 10, then timer expiry callbacks that are executed at time 5990 or later are accepted, all the ones that
 * arrive earlier are ignored.
 */
#define CONFIG_LED_MANAGER_IGNORE_TIMER_MARGIN_MS

/**
 * @brief Defines the number of alert status change messages that can be in the process of being sent at the same time.
 *
 * Message transceiver is an asynchronous interface. The user calls @ref
 * msg_transceiver_send_alert_status_change_message, and gets notified when the message is sent via a callback. The user
 * can call @ref msg_transceiver_send_alert_status_change_message many times in quick succession, which would mean that
 * several "alert status change" messages are now in the progress of being sent.
 *
 * If @ref msg_transceiver_send_alert_status_change_message is called when the maximum allowed number of alert status
 * change messages is already in the process of being sent, that message will fail to get sent.
 */
#define CONFIG_MSG_TRANSCEIVER_MAX_NUM_CONCURRENT_ALERT_STATUS_CHANGE_MESSAGES

/** Defines the buffer size in bytes of the internal ring buffer used in the message queue for the central event queue.
 */
#define CONFIG_CENTRAL_EVENT_QUEUE_MESSAGE_QUEUE_BUF_SIZE

/** Maximum number of linked list nodes that can be simultaneously allocated by the linked list node allocator.
 * Should be set to (CONFIG_MAX_NUM_ALERTS + CONFIG_MAX_TOTAL_NUM_VARIABLE_REQUIREMENTS). We need CONFIG_MAX_NUM_ALERTS
 * nodes for LedManager - one node for each added LedNotification. We also need nodes
 * CONFIG_MAX_TOTAL_NUM_VARIABLE_REQUIREMENTS for variable requirements. */
#define CONFIG_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES

#endif /* ENV_ALERT_SYSTEM_SRC_APP_CONFIG_CONFIG_H */
