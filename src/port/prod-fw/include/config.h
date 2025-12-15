#ifndef ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INCLUDE_CONFIG_H
#define ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INCLUDE_CONFIG_H

#define CONFIG_MAX_NUM_ALERTS 10

/* One for each variable */
#define CONFIG_VALUE_HOLDER_MAX_NUM_INSTANCES 4
#define CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES 1
#define CONFIG_PRESSURE_VALUE_MAX_NUM_INSTANCES 1
#define CONFIG_HUMIDITY_VALUE_MAX_NUM_INSTANCES 1
#define CONFIG_LIGHT_INTENSITY_VALUE_MAX_NUM_INSTANCES 1
/* One for led manager, one for each variable requirement list (four lists, one for each variable) */
#define CONFIG_LINKED_LIST_MAX_NUM_INSTANCES 5
/* One for each variable */
#define CONFIG_VARIABLE_REQUIREMENT_LIST_MAX_NUM_INSTANCES 4
#define CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES CONFIG_MAX_NUM_ALERTS
#define CONFIG_ALERT_RAISER_MAX_NUM_INSTANCES CONFIG_MAX_NUM_ALERTS

/* Chosen through trial and error. If set too low, static asserts will fire. */
#define CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE 16

#define CONFIG_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION 10

#define CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS CONFIG_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION

#define CONFIG_MSG_TRANSCEIVER_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION                                        \
    CONFIG_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION

#define CONFIG_MAX_TOTAL_NUM_VARIABLE_REQUIREMENTS                                                                     \
    (CONFIG_MAX_NUM_ALERTS * CONFIG_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION)

#define CONFIG_ALERT_CONDITIONS_NUM_INSTANCES_TO_CREATE CONFIG_MAX_NUM_ALERTS

#define CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE CONFIG_MAX_NUM_ALERTS

#define CONFIG_ALERT_VALIDATOR_MAX_ALLOWED_ALERT_ID (CONFIG_MAX_NUM_ALERTS - 1)

#define CONFIG_CONNECTIVITY_NOTIFIER_MAX_NUM_ALERTS CONFIG_MAX_NUM_ALERTS

#define CONFIG_LED_NOTIFIER_MAX_NUM_ALERTS CONFIG_MAX_NUM_ALERTS

#define CONFIG_LED_MANAGER_NOTIFICATION_DURATION_SECONDS 5

#define CONFIG_LED_MANAGER_IGNORE_TIMER_MARGIN_MS 10

/** Set to CONFIG_MAX_NUM_ALERTS * 2, since in theory every registered alert can change status at the same time.
 * Multiplied by 2 to be on the safe side and allow for a margin.
 */
#define CONFIG_MSG_TRANSCEIVER_MAX_NUM_CONCURRENT_ALERT_STATUS_CHANGE_MESSAGES (CONFIG_MAX_NUM_ALERTS * 2)

/** Should be plenty to store all events that can in theory happen at the same time */
#define CONFIG_CENTRAL_EVENT_QUEUE_MESSAGE_QUEUE_BUF_SIZE 1024

/* Configs for port-specific modules */

/** The maximal number of nodes that the linked list node allocator module can allocate at the same time.
 * We need CONFIG_MAX_NUM_ALERTS nodes for LedManager - one node for each added LedNotification. We also need nodes for
 * variable requirements. We also need nodes for variable requirements, probably 100 - (CONFIG_MAX_NUM_ALERTS *
 * CONFIG_MAX_NUM_REQUIREMENTS_PER_ALERT), where CONFIG_MAX_NUM_REQUIREMENTS_PER_ALERT is 10. This way, we can always
 * store all variable requirements.
 *
 * TODO: setting this to one to not have enough nodes so that we adjust it according to the calculation.
 */
#define CONFIG_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES 1

/** Only one message queue is used - in the central event queue. */
#define CONFIG_EAS_MESSAGE_QUEUE_MAX_NUM_INSTANCES 1

/** Default value, kind of random */
#define CONFIG_EAS_THREAD_STACK_SIZE 1024

/** Main thread has priority 0 by default. The EAS thread has lower priority than main thread, so that main thread can
 * finish whatever it is doing before we start processing messages in the event queue. */
#define CONFIG_EAS_THREAD_PRIORITY 1

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INCLUDE_CONFIG_H */
