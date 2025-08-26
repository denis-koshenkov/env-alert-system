#ifndef PORT_UNIT_TEST_OFF_TARGET_INCLUDE_CONFIG_H
#define PORT_UNIT_TEST_OFF_TARGET_INCLUDE_CONFIG_H

/** Should correspond to the number of times <module_name>_create() will be called in the unit test program. */
#define CONFIG_VALUE_HOLDER_MAX_NUM_INSTANCES 14
#define CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES 8
#define CONFIG_PRESSURE_VALUE_MAX_NUM_INSTANCES 8
#define CONFIG_HUMIDITY_VALUE_MAX_NUM_INSTANCES 8
#define CONFIG_LIGHT_INTENSITY_VALUE_MAX_NUM_INSTANCES 8
#define CONFIG_LINKED_LIST_MAX_NUM_INSTANCES 3

/* Port-specific configs */
#define CONFIG_MEMORY_BLOCK_ALLOCATOR_MAX_NUM_INSTANCES 5

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

#endif
