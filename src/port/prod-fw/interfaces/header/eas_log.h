#ifndef ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INTERFACES_HEADER_EAS_LOG_H
#define ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INTERFACES_HEADER_EAS_LOG_H

#include <zephyr/logging/log.h>

/** In zephyr, every source file that uses logging should define or declare a zephyr logging module. We use one logging
 * module - eas - for all logs. By calling this at the top of the source file, the source file declares the eas logging
 * module and can then use the logging API by calling EAS_LOG_INF(). */
#define EAS_LOG_ENABLE_IN_FILE() LOG_MODULE_DECLARE(eas)

#define EAS_LOG_INF(...) LOG_INF(__VA_ARGS__)

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INTERFACES_HEADER_EAS_LOG_H */
