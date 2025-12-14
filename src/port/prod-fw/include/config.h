#ifndef ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INCLUDE_CONFIG_H
#define ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INCLUDE_CONFIG_H

/** Maximal size of one variable requirement in bytes. We have different variable requirement types - e.g.
 * TemperatureVariableRequirement, PressureVariableRequirement, and so on. Each of these types might have a different
 * size, because the sizes of data types such as Temperature and Pressure can be different. This config defines the size
 * of the largest variable requirement type.
 *
 * The size of each variable requirement type should be less than or equal to this config. It is the responsibility of
 * each variable requirement module to assert that.
 *
 * TODO: setting this to 1 so that prod fw build fails and we need to adjust this according to the variable requirement
 * sizes on the target hardware.
 */
#define CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE 1

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

/** Should be plenty to store all events that can in theory happen at the same time */
#define CONFIG_CENTRAL_EVENT_QUEUE_MESSAGE_QUEUE_BUF_SIZE 1024

/** Default value, kind of random*/
#define CONFIG_EAS_THREAD_STACK_SIZE 1024

/** Main thread has priority 0 by default. The EAS thread has lower priority than main thread, so that main thread can
 * finish whatever it is doing before we start processing messages in the event queue. */
#define CONFIG_EAS_THREAD_PRIORITY 1

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INCLUDE_CONFIG_H */
