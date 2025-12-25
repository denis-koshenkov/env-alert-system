#include <stdbool.h>

#include "sht31.h"
#include "eas_log.h"

EAS_LOG_ENABLE_IN_FILE();

#define SHT31_SINGLE_MEASUREMENT_DELAY_MS 20

/* Result of (315 / (2^16 - 1)). Part of the formula from the datasheet that converts raw temperature measurement to a
 * value in degrees Celsius. */
#define SHT31_TEMPERATURE_CONVERSION_MAGIC 0.002670328831921f
/* Result of (100 / (2^16 - 1)). Part of the formula from the datasheet that converts raw humidity measurement to a
 * value in RH%. */
#define SHT31_HUMIDITY_CONVERSION_MAGIC 0.001525902189669f

static uint8_t i2c_addr;
static SHT31_I2CRead i2c_read = NULL;
static SHT31_I2CWrite i2c_write = NULL;
static SHT31StartTimer start_timer = NULL;
static SHT31MeasCompleteCb meas_complete_cb = NULL;
static void *meas_complete_cb_user_data = NULL;

/* Temporary, it we keep the buffers static, need to carefully choose max size. */
static uint8_t twim_tx_buffer[2] = {0x1, 0x0};
static uint8_t twim_rx_buffer[6] = {0x1, 0x0, 0x1, 0x0, 0x1, 0x0};

/**
 * @brief Execute measurement complete callback, if one was set.
 *
 * @param meas Measurement to pass to the callback.
 */
static void execute_meas_complete_cb(SHT31Measurement *meas)
{
    if (meas_complete_cb) {
        meas_complete_cb(meas, meas_complete_cb_user_data);
    }
}

/**
 * @brief Convert two bytes in big endian to an integer of type uint16_t.
 *
 * @param bytes The two bytes at this address are used for conversion.
 *
 * @return uint16_t Resulting integer.
 */
static uint16_t two_big_endian_bytes_to_uint16(const uint8_t *const bytes)
{
    return (((uint16_t)(bytes[0])) << 8) | ((uint16_t)(bytes[1]));
}

/**
 * @brief Convert raw temperature measurement to temperature in celsius.
 *
 * @param raw_temp Should point to 2 bytes that are raw temperature measurement read out from the device.
 *
 * @return float Resulting temperature in Celsius.
 */
static float convert_raw_temp_meas_to_celsius(const uint8_t *const raw_temp)
{
    /* Device sends raw measurements in big endian, convert to a 16-bit value in target endianness */
    uint16_t raw_temp_val = two_big_endian_bytes_to_uint16(raw_temp);
    /* Based on conversion formula from the SHT31 datasheet, p. 14, section 4.13. */
    float temperature_celsius = (SHT31_TEMPERATURE_CONVERSION_MAGIC * (float)raw_temp_val) - 45;
    return temperature_celsius;
}

/**
 * @brief Convert raw humidity measurement to humidity in RH%.
 *
 * @param raw_humidity Should point to 2 bytes that are raw humidity measurement read out from the device.
 *
 * @return float Resulting humidity in RH%.
 */
static float convert_raw_humidity_meas_to_rh(const uint8_t *const raw_humidity)
{
    /* Device sends raw measurements in big endian, convert to a 16-bit value in target endianness */
    uint16_t raw_humidity_val = two_big_endian_bytes_to_uint16(raw_humidity);
    /* Based on conversion formula from the SHT31 datasheet, p. 14, section 4.13. */
    float humidity_rh = SHT31_HUMIDITY_CONVERSION_MAGIC * (float)raw_humidity_val;
    return humidity_rh;
}

/* Serves as a I2C transaction complete cb to i2c_write invoked in sht31_read_single_shot_measurement_part_3. */
static void read_single_shot_measurement_part_4(bool result, void *user_data)
{
    if (!result) {
        /* NULL to indicate failure, since the previous I2C transaction failed. */
        execute_meas_complete_cb(NULL);
        return;
    }

    EAS_LOG_INF("Successfully read measurement from device");
    EAS_LOG_HEXDUMP_INF(&(twim_rx_buffer[0]), 2, "Raw temp");
    EAS_LOG_HEXDUMP_INF(&(twim_rx_buffer[3]), 2, "Raw humidity");

    /* twim_rx_buffer now contains the raw measurements. Need to convert them to temperature in Celsius and humidity in
     * RH%. */
    SHT31Measurement meas;
    /* Temperature is the first two bytes in the received data. */
    meas.temperature = convert_raw_temp_meas_to_celsius(&(twim_rx_buffer[0]));
    /* Bytes 3 and 4 in the received data form the raw humidity measurement. */
    meas.humidity = convert_raw_humidity_meas_to_rh(&(twim_rx_buffer[3]));

    /* Measurement readout complete, execute user-provided measurement complete callback. */
    execute_meas_complete_cb(&meas);
}

/* Serves as the SHT31 timer expired callback to start_timer invoked in read_single_shot_measurement_part_3. Reads
 * measurement results from the device. */
static void read_single_shot_measurement_part_3(void *user_data)
{
    EAS_LOG_INF("20 ms passed, reading measurement from device");
    i2c_read(twim_rx_buffer, 6, i2c_addr, read_single_shot_measurement_part_4, NULL);
}

/* Serves as a I2C transaction complete cb to i2c_write invoked in sht31_read_single_shot_measurement. Starts a 20 ms
 * timer if the previous transaction succeeded. */
static void read_single_shot_measurement_part_2(bool result, void *user_data)
{
    if (!result) {
        /* NULL to indicate failure, since the previous I2C transaction failed. */
        execute_meas_complete_cb(NULL);
        return;
    }

    EAS_LOG_INF("Single shot measurement cmd successful, waiting for 20 ms");
    /* We sent the measurement command, now wait for device to perform the measurement */
    start_timer(SHT31_SINGLE_MEASUREMENT_DELAY_MS, read_single_shot_measurement_part_3, NULL);
}

void sht31_init(const SHT31InitConfig *const cfg)
{
    i2c_read = cfg->i2c_read;
    i2c_write = cfg->i2c_write;
    /* Can add a check that address is either 0x44 or 0x45, as these are the only two allowed values */
    i2c_addr = cfg->i2c_addr;
    start_timer = cfg->start_timer;
}

void sht31_read_single_shot_measurement(SHT31MeasCompleteCb cb, void *user_data)
{
    meas_complete_cb = cb;
    meas_complete_cb_user_data = user_data;

    /* Write command to read out high repeateability single-shot measurement with clock stretching disabled. Execute
     * read_single_shot_measurement_part_2 once the transaction is complete. */
    twim_tx_buffer[0] = 0x24;
    twim_tx_buffer[1] = 0x0;
    i2c_write(twim_tx_buffer, 2, i2c_addr, read_single_shot_measurement_part_2, NULL);
    EAS_LOG_INF("Sent single shot measurement cmd");
}
