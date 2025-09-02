#ifndef ENV_ALERT_SYSTEM_SRC_APP_ALERT_RAISER_DEFS_H
#define ENV_ALERT_SYSTEM_SRC_APP_ALERT_RAISER_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Alert raiser public definitions.
 *
 * The definition of the AlertRaiser type is here, so that other modules can use it without including the whole
 * alert_raiser.h header.
 *
 * For example, the alert raisers module only needs to know about the AlertRaiser type definition, it should
 * not know about and expose the whole public API defined in alert_raiser.h.
 */

typedef struct AlertRaiserStruct *AlertRaiser;

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_ALERT_RAISER_DEFS_H */
