#include "eas_log.h"

/** All logging is done from the same zephyr logging module. This is in a .c file instead of .h, because this should be
 * done only once across all translation units. If we put it in a header file, it will be present in every translation
 * units that includes eas_log.h.*/
LOG_MODULE_REGISTER(eas);
