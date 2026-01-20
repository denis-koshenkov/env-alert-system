#ifndef ENV_ALERT_SYSTEM_SRC_HAL_IMPL_NRF52840DK_HW_PLATFORM_PRIVATE_H
#define ENV_ALERT_SYSTEM_SRC_HAL_IMPL_NRF52840DK_HW_PLATFORM_PRIVATE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "nrfx_twim.h"

#include "sht3x.h"
#include "bh1750.h"
#include "eas_timer.h"

typedef enum {
    /** Callback is of type SHT3X_I2CTransactionCompleteCb. */
    I2C_COMPLETE_CB_TYPE_SHT3X,
    /** Callback is of type BH1750_I2CCompleteCb. */
    I2C_COMPLETE_CB_TYPE_BH1750,
} I2cCompleteCbType;

typedef union {
    /** This field holds the value if cb_type field in I2cCompleteCbData is I2C_COMPLETE_CB_TYPE_SHT3X. */
    SHT3X_I2CTransactionCompleteCb sht3x_cb;
    /** This field holds the value if cb_type field in I2cCompleteCbData is I2C_COMPLETE_CB_TYPE_BH1750. */
    BH1750_I2CCompleteCb bh1750_cb;
} I2cCompleteCb;

typedef struct {
    /** Callback to execute when the I2C transaction is complete. */
    I2cCompleteCb cb;
    /** User data to pass to cb. */
    void *user_data;
    /** One of @ref I2cCompleteCbType. Defines what type of callback cb is. */
    uint8_t cb_type;
} I2cCompleteCbData;

/**
 * @brief All data related to a I2C operation.
 *
 * The callback stored in complete_cb_data.cb must be executed once the I2C transaction is complete.
 *
 * This struct is defined in a separate header, because it needs to be used in our implementations of
 * i2c_read and i2c_write for SHT3X and BH1750 driver functions, located in drivers_glue directory.
 *
 * However, this struct also needs to be used in hw_platform.c in order to define and use the I2C operations queue.
 *
 * This header is then included both by hw_platform.c and the interface implementations for sensor drivers, located in
 * drivers_glue directory.
 *
 * How the data is used:
 * 1. i2c_read or i2c_write is called by SHT3X or BH1750 driver.
 * 2. The implementations of these functions populate all fields of this struct to define what kind of I2C operation
 * must be performed. The I2C operation is then submitted to a I2C operations queue.
 * 3. I2C operations queue calls the used-defined function to start the operation (defined in hw_platform.c) - either
 * right away if there are no ongoing I2C operations, or at a later point once all previously scheduled I2C operations
 * are performed.
 * 4. The user-defined function to start the I2C operation saves the complete_cb_data field of this struct to a private
 * hw_platform variable. This is needed so that once the I2C operation is complete, hw_platform knows which I2C complete
 * callback to execute.
 * 5. The user-defined function to start the I2C operation starts the I2C operation using a NRF TWIM instance.
 * 6. I2C transaction is complete. TWIM ISR handler pushes a callback defined in hw_platform.c to the event queue.
 * 7. The event queue handler invokes the hw_platform callback from the previous step, which in turn invokes the
 * callback with user data. It takes the callback from the private hw_platform variable described in step 4.
 */
typedef struct {
    /** NRF TWIM I2C transaction descriptor. */
    nrfx_twim_xfer_desc_t twim_xfer_desc;
    /** All necessary data to execute a complete callback for this operation. */
    I2cCompleteCbData complete_cb_data;
} I2cOperation;

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
