#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INCLUDE_CONFIG_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INCLUDE_CONFIG_H

#define CONFIG_VALUE_HOLDER_MAX_NUM_INSTANCES 14
#define CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES 8
#define CONFIG_PRESSURE_VALUE_MAX_NUM_INSTANCES 8
#define CONFIG_HUMIDITY_VALUE_MAX_NUM_INSTANCES 8
#define CONFIG_LIGHT_INTENSITY_VALUE_MAX_NUM_INSTANCES 8
#define CONFIG_LINKED_LIST_MAX_NUM_INSTANCES 49
#define CONFIG_VARIABLE_REQUIREMENT_LIST_MAX_NUM_INSTANCES 10
#define CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES 1
#define CONFIG_ALERT_RAISER_MAX_NUM_INSTANCES 27
/* This config has no effect on the behavior of the unit test port. The eas timer implementation for this port is a
 * mock, so it does not define a static array of size equal to the maximum number of instances. */
#define CONFIG_EAS_TIMER_MAX_NUM_INSTANCES 1

/** For the off-target unit test build, this config is used in mock variable requirement allocator to determine the size
 * of the buffer for one variable requirement. */
#define CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE 24

#define CONFIG_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION 10

#define CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS CONFIG_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION

#define CONFIG_MSG_TRANSCEIVER_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION                                        \
    CONFIG_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION

#define CONFIG_MAX_NUM_ALERTS 10

#define CONFIG_MAX_TOTAL_NUM_VARIABLE_REQUIREMENTS                                                                     \
    (CONFIG_MAX_NUM_ALERTS * CONFIG_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ALERT_CONDITION)

/* This config has no effect on the behavior of the unit test port. This port implements two version of variable
 * requirement allocator: mock and fake. Mock simply records function calls, so it does not define any memory for the
 * allocated requirements. The fake uses its own config, CONFIG_FAKE_VARIABLE_REQUIREMENT_ALLOCATOR_NUM_REQUIREMENTS. */
#define CONFIG_VARIABLE_REQUIREMENT_ALLOCATOR_NUM_REQUIREMENTS 1

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

/** It is defined here, but not actually used since central event queue is not used in the unit test port. */
#define CONFIG_CENTRAL_EVENT_QUEUE_MESSAGE_QUEUE_BUF_SIZE 1024

/* This config has no effect on the behavior of the unit test port. This port implements two version of linked list node
 * allocator: mock and fake. Mock simply records function calls, so it does not define any memory for the allocated
 * nodes. The fake uses its own config, CONFIG_FAKE_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES. */
#define CONFIG_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES 1

/* This config has no effect on the behavior of the unit test port. This port implements a mock object for the
 * led_notification_allocator interface. The mock does not define any memory for the allocated notifications. */
#define CONFIG_LED_NOTIFICATION_ALLOCATOR_NUM_NOTIFICATIONS 1

/* Configs for port-specific modules */

/** Should correspond to the number of times <module_name>_create() will be called in the unit test program. */
#define CONFIG_MEMORY_BLOCK_ALLOCATOR_MAX_NUM_INSTANCES 8

/** The maximal number of nodes that the fake linked list node allocator module can allocate at the same time. */
#define CONFIG_FAKE_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES 7

/** The maximal number of requirements that the fake variable requirement allocator module can allocate at the same
 * time. */
#define CONFIG_FAKE_VARIABLE_REQUIREMENT_ALLOCATOR_NUM_REQUIREMENTS 10

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_INCLUDE_CONFIG_H */
