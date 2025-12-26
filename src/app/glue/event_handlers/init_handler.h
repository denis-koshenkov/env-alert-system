#ifndef ENV_ALERT_SYSTEM_SRC_APP_GLUE_EVENT_HANDLERS_INIT_HANDLER_H
#define ENV_ALERT_SYSTEM_SRC_APP_GLUE_EVENT_HANDLERS_INIT_HANDLER_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Perform part 1 of system initialization.
 *
 * This function is called as a handler for the "init" event. This should only happen once, on the bootup of the
 * application.
 */
void init_handler_handle_init_event();

/**
 * @brief Perform part 2 of system initialization.
 *
 * This function is called as a handler for the "init part 2" event. This function must be called after hw platform
 * intialization (triggered in part 1) is finished.
 */
void init_handler_handle_init_part_2_event();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_GLUE_EVENT_HANDLERS_INIT_HANDLER_H */
