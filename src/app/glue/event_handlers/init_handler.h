#ifndef ENV_ALERT_SYSTEM_SRC_APP_GLUE_EVENT_HANDLERS_INIT_HANDLER_H
#define ENV_ALERT_SYSTEM_SRC_APP_GLUE_EVENT_HANDLERS_INIT_HANDLER_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Initialize all application modules.
 *
 * This function is called as a handler for the "init" event. This should only happen once, on the bootup of the
 * application.
 */
void init_handler_handle_init_event();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_GLUE_EVENT_HANDLERS_INIT_HANDLER_H */
