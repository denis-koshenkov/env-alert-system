#ifndef PORT_UNIT_TEST_OFF_TARGET_INCLUDE_CONFIG_H
#define PORT_UNIT_TEST_OFF_TARGET_INCLUDE_CONFIG_H

/** Should correspond to the number of times <module_name>_create() will be called in the unit test program. */
#define CONFIG_VALUE_HOLDER_MAX_NUM_INSTANCES 14
#define CONFIG_TEMPERATURE_VALUE_MAX_NUM_INSTANCES 8
#define CONFIG_PRESSURE_VALUE_MAX_NUM_INSTANCES 8
#define CONFIG_HUMIDITY_VALUE_MAX_NUM_INSTANCES 8
#define CONFIG_LIGHT_INTENSITY_VALUE_MAX_NUM_INSTANCES 8

/// Defines the number of blocks in the block allocator for variable requirements
#define CONFIG_VARIABLE_REQUIREMENT_ALLOCATOR_MAX_NUM_VARIABLE_REQUIREMENTS 10

#endif
