#include "led_gpio.h"
#include "elog.h"
#include <stdlib.h>

static const char *TAG = "LED_GPIO";

typedef struct {
    led_type_t type;
    led_gpio_config_t config;
    uint8_t current_brightness;
} led_gpio_t;

led_handle_t led_gpio_create(const led_gpio_config_t *config)
{
    led_gpio_t *led = NULL;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (config == NULL) {
        elog_e(TAG, "GPIO LED config is NULL");
        return NULL;
    }

    led = (led_gpio_t *)malloc(sizeof(led_gpio_t));
    if (led == NULL) {
        elog_e(TAG, "Failed to allocate memory for GPIO LED");
        return NULL;
    }

    /* 保存配置 */
    led->type = LED_TYPE_GPIO;
    led->config = *config;
    led->current_brightness = 0;

    /* 配置GPIO */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pin = config->gpio_pin;

    HAL_GPIO_Init(config->gpio_port, &GPIO_InitStruct);

    /* 初始状态设为关闭 */
    HAL_GPIO_WritePin(config->gpio_port, config->gpio_pin, 
                     config->active_level ? GPIO_PIN_RESET : GPIO_PIN_SET);

    elog_i(TAG, "GPIO LED created");
    return (led_handle_t)led;
}

HAL_StatusTypeDef led_gpio_delete(led_handle_t led_handle)
{
    led_gpio_t *led = (led_gpio_t *)led_handle;

    if (led == NULL) {
        return HAL_ERROR;
    }

    /* 关闭LED */
    HAL_GPIO_WritePin(led->config.gpio_port, led->config.gpio_pin, 
                     led->config.active_level ? GPIO_PIN_RESET : GPIO_PIN_SET);

    /* 释放内存 */
    free(led);
    return HAL_OK;
}

HAL_StatusTypeDef led_gpio_set_brightness(led_handle_t led_handle, uint8_t brightness)
{
    led_gpio_t *led = (led_gpio_t *)led_handle;
    GPIO_PinState pin_state;

    if (led == NULL) {
        return HAL_ERROR;
    }

    /* GPIO模式只支持开关两种状态 */
    if (brightness > 127) {
        pin_state = led->config.active_level ? GPIO_PIN_SET : GPIO_PIN_RESET;
        led->current_brightness = 255;
    } else {
        pin_state = led->config.active_level ? GPIO_PIN_RESET : GPIO_PIN_SET;
        led->current_brightness = 0;
    }

    HAL_GPIO_WritePin(led->config.gpio_port, led->config.gpio_pin, pin_state);
    return HAL_OK;
}

uint8_t led_gpio_get_brightness(led_handle_t led_handle)
{
    led_gpio_t *led = (led_gpio_t *)led_handle;

    if (led == NULL) {
        return 0;
    }

    return led->current_brightness;
} 
