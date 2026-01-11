#ifndef ENV_ALERT_SYSTEM_SRC_HAL_NRF52840DK_DRIVERS_GLUE_SHT3X_GET_INSTANCE_MEMORY_H
#define ENV_ALERT_SYSTEM_SRC_HAL_NRF52840DK_DRIVERS_GLUE_SHT3X_GET_INSTANCE_MEMORY_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Implementation of get_instance_memory for SHT3X driver.
 *
 * @param user_data User data.
 *
 * @return void* Pointer to memory of sizeof(struct SHT3XStruct).
 */
void *sht3x_driver_get_instance_memory(void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_HAL_NRF52840DK_DRIVERS_GLUE_SHT3X_GET_INSTANCE_MEMORY_H */
