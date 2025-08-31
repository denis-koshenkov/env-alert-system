#ifndef PORT_UNIT_TEST_OFF_TARGET_INCLUDE_CONFIG_H
#define PORT_UNIT_TEST_OFF_TARGET_INCLUDE_CONFIG_H

/** Should correspond to the number of times <module_name>_create() will be called in the unit test program. */
#define CONFIG_VALUE_HOLDER_MAX_NUM_INSTANCES 14
#define CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES 8
#define CONFIG_PRESSURE_VALUE_MAX_NUM_INSTANCES 8
#define CONFIG_HUMIDITY_VALUE_MAX_NUM_INSTANCES 8
#define CONFIG_LIGHT_INTENSITY_VALUE_MAX_NUM_INSTANCES 8
#define CONFIG_LINKED_LIST_MAX_NUM_INSTANCES 27
#define CONFIG_VARIABLE_REQUIREMENT_LIST_MAX_NUM_INSTANCES 10
#define CONFIG_ALERT_CONDITION_MAX_NUM_INSTANCES 20

/** Maximal size of one variable requirement in bytes. We have different variable requirement types - e.g.
 * TemperatureVariableRequirement, PressureVariableRequirement, and so on. Each of these types might have a different
 * size, because the sizes of data types such as Temperature and Pressure can be different. This config defines the size
 * of the largest variable requirement type.
 *
 * The size of each variable requirement type should be less than or equal to this config. It is the responsibility of
 * each variable requirement module to assert that.
 *
 * For the off-target unit test build, this config is used in mock variable requirement allocator to determine the size
 * of the buffer for one variable requirement.
 */
#define CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE 24

/** Maximal allowed number of variable requirements in one alert condition. */
#define CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS 10

/* Configs for port-specific modules */

/** Should correspond to the number of times <module_name>_create() will be called in the unit test program. */
#define CONFIG_MEMORY_BLOCK_ALLOCATOR_MAX_NUM_INSTANCES 8

/** The maximal number of nodes that the fake linked list node allocator module can allocate at the same time. */
#define CONFIG_FAKE_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES 7

/** The maximal number of requirements that the fake variable requirement allocator module can allocate at the same
 * time. */
#define CONFIG_FAKE_VARIABLE_REQUIREMENT_ALLOCATOR_NUM_REQUIREMENTS 10

#endif
