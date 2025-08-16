#ifndef PORT_PROD_FW_INCLUDE_CONFIG_H
#define PORT_PROD_FW_INCLUDE_CONFIG_H

/// Maximal number of alerts that can be registered in the system at the same time.
#define CONFIG_MAX_NUM_ALERTS 10

/// Maximal number of variable requirements allowed in one alert condition.
#define CONFIG_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ONE_ALERT 10

/** Defines the total number of variable requirements that the system needs to be able to store at the same. Defines the
 * number of blocks in the block allocator for variable requirements. */
#define CONFIG_VARIABLE_REQUIREMENT_ALLOCATOR_MAX_NUM_VARIABLE_REQUIREMENTS                                            \
    (CONFIG_MAX_NUM_ALERTS * CONFIG_MAX_NUM_VARIABLE_REQUIREMENTS_IN_ONE_ALERT)

#endif
