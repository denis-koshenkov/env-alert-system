#include <math.h>

#include <zephyr/drivers/pwm.h>

#include "virtual_led_nrf52840.h"
#include "eas_assert.h"

static void led_set(LedColor led_color);
static void led_turn_off();

static Led led = {
    .set = led_set,
    .turn_off = led_turn_off,
};

static const ZephyrPwms *pwms = NULL;

/**
 * @brief Convert RGB LED color to its red, green, and blue components.
 *
 * @param[in] led_color Led color.
 * @param[out] red Red component, range 0.0f - 1.0f.
 * @param[out] green Green component, range 0.0f - 1.0f.
 * @param[out] blue Blue component, range 0.0f - 1.0f.
 */
static void led_color_to_rgb_values(LedColor led_color, float *const red, float *const green, float *const blue)
{
    EAS_ASSERT(red);
    EAS_ASSERT(green);
    EAS_ASSERT(blue);

    switch (led_color) {
    case LED_COLOR_RED:
        *red = 1.0f;
        *green = 0.0f;
        *blue = 0.0f;
        break;
    case LED_COLOR_GREEN:
        *red = 0.0f;
        *green = 1.0f;
        *blue = 0.0f;
        break;
    case LED_COLOR_BLUE:
        *red = 0.0f;
        *green = 0.0f;
        *blue = 1.0f;
        break;
    default:
        /* Invalid led color */
        EAS_ASSERT(0);
        break;
    }
}

/**
 * @brief Check if color component is valid, i.e. a float between 0 and 1.
 *
 * @param[in] color_component Color component value.
 *
 * @retval true Color component is valid.
 * @retval false Color component is invalid.
 */
static bool is_valid_color_component(float color_component)
{
    return (color_component >= 0.0f && color_component <= 1.0f);
}

/**
 * @brief Set PWM LEDs according to color components.
 *
 * @param[in] red Red component, range 0.0f - 1.0f.
 * @param[in] green Green component, range 0.0f - 1.0f.
 * @param[in] blue Blue component, range 0.0f - 1.0f.
 */
static void set_pwm(float red, float green, float blue)
{
    EAS_ASSERT(pwms);
    EAS_ASSERT(is_valid_color_component(red));
    EAS_ASSERT(is_valid_color_component(green));
    EAS_ASSERT(is_valid_color_component(blue));

    uint32_t pulse_red = lroundf(red * pwms->red_pwm_led.period);
    uint32_t pulse_green = lroundf(green * pwms->green_pwm_led.period);
    uint32_t pulse_blue = lroundf(blue * pwms->blue_pwm_led.period);

    int rc;
    rc = pwm_set_pulse_dt(&pwms->red_pwm_led, pulse_red);
    EAS_ASSERT(rc == 0);
    rc = pwm_set_pulse_dt(&pwms->green_pwm_led, pulse_green);
    EAS_ASSERT(rc == 0);
    rc = pwm_set_pulse_dt(&pwms->blue_pwm_led, pulse_blue);
    EAS_ASSERT(rc == 0);
}

static void led_set(LedColor led_color)
{
    EAS_ASSERT(pwms);
    float red, green, blue;
    led_color_to_rgb_values(led_color, &red, &green, &blue);
    set_pwm(red, green, blue);
}

static void led_turn_off()
{
    EAS_ASSERT(pwms);
    set_pwm(0.0f, 0.0f, 0.0f);
}

Nrf52840LedVirtualInterfaces virtual_led_nrf52840_initialize(const ZephyrPwms *const zephyr_pwms)
{
    EAS_ASSERT(zephyr_pwms);
    pwms = zephyr_pwms;
    return (Nrf52840LedVirtualInterfaces){.led = &led};
}
