/**
 * @file button_gpio.c
 * @brief STM32 GPIO按键驱动实现
 */

#include "button_gpio.h"

HAL_StatusTypeDef button_gpio_init(const button_gpio_config_t *config)
{
    if (config == NULL || config->gpio_port == NULL) {
        return HAL_ERROR;
    }

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 配置GPIO为输入模式 */
    GPIO_InitStruct.Pin = config->gpio_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    
    /* 根据有效电平配置上下拉 */
    if (config->active_level) {
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    } else {
        GPIO_InitStruct.Pull = GPIO_PULLUP;
    }
    
    /* 初始化GPIO */
    HAL_GPIO_Init(config->gpio_port, &GPIO_InitStruct);

    return HAL_OK;
}

HAL_StatusTypeDef button_gpio_deinit(const button_gpio_config_t *config)
{
    if (config == NULL || config->gpio_port == NULL) {
        return HAL_ERROR;
    }

    /* 复位GPIO配置 */
    HAL_GPIO_DeInit(config->gpio_port, config->gpio_pin);

    return HAL_OK;
}

uint8_t button_gpio_get_key_level(const button_gpio_config_t *config)
{
    if (config == NULL || config->gpio_port == NULL) {
        return 0;
    }

    /* 读取GPIO电平 */
    return (uint8_t)HAL_GPIO_ReadPin(config->gpio_port, config->gpio_pin);
} 
