#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <stdbool.h>

#include "hal/temperature.h"

/**
 * @brief Stores the current temperature value.
 *
 * Contract:
 *   - get() should never be called before set() is called at least once.
 *   - is_value_changed() should never be called before set() is called at least once.
 *   - init() should be the first function to be called. It is not allowed to call any other function before init().
 */
struct temperature_value {
    temperature value;
    bool initialized;         /**< True if init() has been called. */
    bool set_has_been_called; /**< True if set() has been called at least once, false otherwise. */
    bool value_changed;
};

void temperature_value_init(struct temperature_value *temperature_value);

void temperature_value_set(struct temperature_value *temperature_value, temperature value);

temperature temperature_value_get(struct temperature_value *temperature_value);

bool temperature_value_is_value_changed(struct temperature_value *temperature_value);

#endif
