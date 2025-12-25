#ifndef ENV_ALERT_SYSTEM_SRC_PORT_NRF52840DK_INTERFACES_HAL_DRIVERS_SHT31_H
#define ENV_ALERT_SYSTEM_SRC_PORT_NRF52840DK_INTERFACES_HAL_DRIVERS_SHT31_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Callback type to execute when a I2C transaction to SHT31 is complete.
 *
 * @param result Must be true if transaction was successful, false otherwise.
 * @param user_data The caller must pass user_data parameter that the SHT31 driver passed to @ref SHT31_I2CRead or @ref
 * SHT31_I2CWrite.
 */
typedef void (*SHT31_I2CTransactionCompleteCb)(bool result, void *user_data);

/**
 * @brief Perform a I2C read transaction to the SHT31 device.
 *
 * @param[out] data Data that is read from the device is written to this parameter in case of success. Success or
 * failure is indicated via a parameter to @p cb.
 * @param[in] length Number of bytes in the @p data array.
 * @param[in] i2c_addr I2C address of the SHT31 device.
 * @param[in] cb Callback to execute once the I2C transaction is complete. This callback must be executed from the
 * same context that the SHT31 driver API functions get called from.
 * @param[in] user_data User data to pass to @p cb.
 */
typedef void (*SHT31_I2CRead)(uint8_t *data, size_t length, uint8_t i2c_addr, SHT31_I2CTransactionCompleteCb cb,
                              void *user_data);

/**
 * @brief Perform a I2C write transaction to the SHT31 device.
 *
 * @param[in] data Data to write to the device.
 * @param[in] length Number of bytes in the @p data array.
 * @param[in] i2c_addr I2C address of the SHT31 device.
 * @param[in] cb Callback to execute once the I2C transaction is complete. This callback must be executed from the
 * same context that the SHT31 driver API functions get called from.
 * @param[in] user_data User data to pass to @p cb.
 */
typedef void (*SHT31_I2CWrite)(uint8_t *data, size_t length, uint8_t i2c_addr, SHT31_I2CTransactionCompleteCb cb,
                               void *user_data);

/**
 * @brief Definition of callback type to execute when a SHT31 timer expires.
 *
 * @param user_data User data that was passed to the user_data parameter of @ref SHT31StartTimer.
 */
typedef void (*SHT31TimerExpiredCb)(void *user_data);

/**
 * @brief Execute @p cb after @p duration_ms ms pass.
 *
 * The driver calls this function when it needs to have a delay between two actions. For example, a command was sent to
 * the device, and the result of the command will be available only after some time. The driver will call this function
 * after it sent the command, and @p cb will contain the implementation of reading the result of the command. The
 * implementation of this callback should call @p cb after at least @p duration_ms pass.
 *
 * @p cb must be executed from the same execution context as all other driver functions are called from.
 *
 * @param duration_ms @p cb must be called after at least this number of ms pass.
 * @param cb Callback to execute.
 * @param user_data User data to pass to the callback.
 */
typedef void (*SHT31StartTimer)(uint32_t duration_ms, SHT31TimerExpiredCb cb, void *user_data);

/** Represents a single measurement that can be read out from the device. */
typedef struct {
    float temperature; /**< Temperature in degress celsius. */
    float humidity;    /**< Humidity in RH%. */
} SHT31Measurement;

/**
 * @brief Callback type to execute when the driver finishes reading out a measurement.
 *
 * @param meas Measurement that was read out. Contains both temperature and humidity. NULL if failed to read out the
 * measurement.
 * @param user_data User data pointer that was passed to user_data parameter of @ref sht31_read_single_shot_measurement.
 */
typedef void (*SHT31MeasCompleteCb)(SHT31Measurement *meas, void *user_data);

typedef struct {
    SHT31_I2CRead i2c_read;
    SHT31_I2CWrite i2c_write;
    SHT31StartTimer start_timer;
    uint8_t i2c_addr;
} SHT31InitConfig;

void sht31_init(const SHT31InitConfig *const cfg);

void sht31_read_single_shot_measurement(SHT31MeasCompleteCb cb, void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_NRF52840DK_INTERFACES_HAL_DRIVERS_SHT31_H */
