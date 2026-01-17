#ifndef ENV_ALERT_SYSTEM_SRC_HAL_IMPL_NRF52840DK_HW_PLATFORM_PRIVATE_H
#define ENV_ALERT_SYSTEM_SRC_HAL_IMPL_NRF52840DK_HW_PLATFORM_PRIVATE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "nrfx_twim.h"

#include "sht3x.h"
#include "eas_timer.h"

/**
 * @brief All data related to SHT3X driver I2C transaction complete callback.
 *
 * The callback stored in the cb field needs to be executed once our implementation of sht3x_driver_i2c_read or
 * sht3x_driver_i2c_write completed the requested I2C transaction.
 *
 * This struct is defined in a separate header, because it needs to be used in our implementations of
 * sht3x_driver_i2c_read and sht3x_driver_i2c_write functions, located in drivers_glue/sht3x/interface.c.
 *
 * However, this struct also needs to be used in hw_platform.c in order to submit cb and user_data to the central event
 * queue from inside the NRF TWIM ISR.
 *
 * This header is then included both by drivers_glue/sht3x/interface.c and hw_platform.c.
 *
 * How the data is used:
 * 1. sht3x_driver_i2c_read or sht3x_driver_i2c_write is called by the SHT3X driver.
 * 2. The implementations of these functions populate the cb and user_data fields. cb is the callback that should be
 * invoked once I2C transaction is complete.
 * 3. sht3x_driver_i2c_read/sht3x_driver_i2c_write starts the I2C transaction using the NRF TWIM instance stored in
 * p_twim_inst.
 * 4. I2C transaction is complete. TWIM ISR handler pushes a callback defined in hw_platform.c to the event queue.
 * 5. The event queue handler invokes the hw_platform callback from the previous step, which in turn invokes cb with
 * user_data as a parameter.
 */
typedef struct {
    /** @brief Callback that should be executed when the I2C transaction is complete. */
    SHT3X_I2CTransactionCompleteCb cb;
    /** @brief User data to pass to cb when it is invoked. Volatile for the same reason as cb. */
    void *user_data;
    /** @brief Pointer to NRF TWIM instance to use for the I2C transaction. */
    nrfx_twim_t *p_twim_inst;
} SHT3XDriverI2cData;

/**
 * @brief All data related to the implementation of sht3x_driver_timer_start function that we pass to SHT3X driver.
 *
 * The callback stored in the cb field needs to be executed once the timer expires. The eas timer that p_eas_timer_inst
 * points to is used to implement this timer.
 *
 * This struct is defined in a separate header, because it needs to be used in our implementation of
 * sht3x_driver_timer_start function, located in drivers_glue/sht3x/interface.c.
 *
 * However, this struct also needs to be used in hw_platform.c so that a static instance of this struct can be defined
 * and passed to the SHT3X init config as timer_start_user_data.
 *
 * How the data is used:
 * 1. sht3x_driver_timer_start is called by the SHT3X driver.
 * 2. The implementation of that function populates the cb and user_data fields. cb is the callback that should be
 * invoked once the time expires.
 * 3. sht3x_driver_timer_start starts the timer using the EAS timer instance that p_eas_timer_inst points to.
 * 4. Timer expires. Timer expiry function accesses cb and user_data fields of this struct, and executes cb with
 * user_data as parameter. All EAS timer expiry functions are executed from the central event queue thread, so this is
 * safe.
 */
typedef struct {
    /** @brief Callback that should be executed when the timer expires. */
    SHT3XTimerExpiredCb cb;
    /** @brief User data to pass to cb when it is invoked. */
    void *user_data;
    /** @brief Pointer to EAS timer instance to use for the timer. */
    const EasTimer *const p_eas_timer_inst;
} SHT3XTimerData;

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_HAL_IMPL_NRF52840DK_HW_PLATFORM_PRIVATE_H */
