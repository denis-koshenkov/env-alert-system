#ifndef ENV_ALERT_SYSTEM_SRC_HAL_IMPL_NRF52840DK_DRIVERS_GLUE_BMP280_GET_INST_BUF_H
#define ENV_ALERT_SYSTEM_SRC_HAL_IMPL_NRF52840DK_DRIVERS_GLUE_BMP280_GET_INST_BUF_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Implementation of get_inst_buf for BMP280 driver.
 *
 * @param user_data User data.
 *
 * @return void* Pointer to memory of sizeof(struct BMP280Struct).
 */
void *bmp280_driver_get_inst_buf(void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_HAL_IMPL_NRF52840DK_DRIVERS_GLUE_BMP280_GET_INST_BUF_H */
