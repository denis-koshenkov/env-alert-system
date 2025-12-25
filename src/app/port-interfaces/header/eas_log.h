#ifndef ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HEADER_EAS_LOG_H
#define ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HEADER_EAS_LOG_H

/**
 * @brief Log messages to a terminal.
 *
 * # Usage
 * At the top of every source file (after all the includes) that uses logging, place EAS_LOG_ENABLE_IN_FILE().
 * Then, inside functions, call EAS_LOG_INF() with printf-style argument formatting to perform logging.
 */

/** All source files that use logging should call EAS_LOG_ENABLE_IN_FILE() at the top. This is necessary for the ports
 * using zephyr, because every source file that calls the zephyr logging API should define or declare a logging module.
 */
#define EAS_LOG_ENABLE_IN_FILE()

/** Log macro. Uses printf-style argument formatting. */
#define EAS_LOG_INF(...)

// TODO: keep? Add docs?
#define EAS_LOG_HEXDUMP_INF(data, length, string)

#endif /* ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_HEADER_EAS_LOG_H */
