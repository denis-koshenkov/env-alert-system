#ifndef ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INCLUDE_EAS_ASSERT_H
#define ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INCLUDE_EAS_ASSERT_H

// TODO: decide what EAS_ASSERT does when we are in prod fw
#define EAS_ASSERT(expr) ((expr) ? (void)(0) : (void)(0))

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_PROD_FW_INCLUDE_EAS_ASSERT_H */
