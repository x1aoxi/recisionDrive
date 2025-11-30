#include "led.h"
#include "led_gpio.h"
#include "led_pwm.h"
#include "elog.h"

static const char *TAG = "LED";

led_handle_t led_create(const led_config_t *config)
{
    led_handle_t handle = NULL;

    if (config == NULL) {
        elog_e(TAG, "LED config is NULL");
        return NULL;
    }

    switch (config->type) {
        case LED_TYPE_GPIO:
            handle = led_gpio_create(&config->led_union.gpio_led_config);
            break;
        case LED_TYPE_PWM:
            handle = led_pwm_create(&config->led_union.pwm_led_config);
            break;
        default:
            elog_e(TAG, "Unknown LED type: %d", config->type);
            break;
    }

    return handle;
}

HAL_StatusTypeDef led_delete(led_handle_t led_handle)
{
    led_config_t *config = (led_config_t *)led_handle;
    
    if (config == NULL) {
        return HAL_ERROR;
    }

    switch (config->type) {
        case LED_TYPE_GPIO:
            return led_gpio_delete(led_handle);
        case LED_TYPE_PWM:
            return led_pwm_delete(led_handle);
        default:
            return HAL_ERROR;
    }
}

HAL_StatusTypeDef led_set_brightness(led_handle_t led_handle, uint8_t brightness)
{
    led_config_t *config = (led_config_t *)led_handle;
    
    if (config == NULL) {
        return HAL_ERROR;
    }

    switch (config->type) {
        case LED_TYPE_GPIO:
            return led_gpio_set_brightness(led_handle, brightness);
        case LED_TYPE_PWM:
            return led_pwm_set_brightness(led_handle, brightness);
        default:
            return HAL_ERROR;
    }
}

uint8_t led_get_brightness(led_handle_t led_handle)
{
    led_config_t *config = (led_config_t *)led_handle;
    
    if (config == NULL) {
        return 0;
    }

    switch (config->type) {
        case LED_TYPE_GPIO:
            return led_gpio_get_brightness(led_handle);
        case LED_TYPE_PWM:
            return led_pwm_get_brightness(led_handle);
        default:
            return 0;
    }
} 
