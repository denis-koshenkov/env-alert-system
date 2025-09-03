#include "hal/hw_platform.h"
#include "virtual_led_mock.h"

const Led *hw_platform_get_led()
{
    return virtual_led_mock_get();
}
