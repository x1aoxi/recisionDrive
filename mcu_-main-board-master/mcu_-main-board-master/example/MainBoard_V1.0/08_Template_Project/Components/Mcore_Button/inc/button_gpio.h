/**
 * @file button_gpio.h
 * @brief STM32 GPIO按键驱动头文件
 */

#ifndef __BUTTON_GPIO_H
#define __BUTTON_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

/**
 * @brief GPIO按键配置结构体
 */
typedef struct {
    GPIO_TypeDef* gpio_port;    // GPIO端口
    uint16_t gpio_pin;          // GPIO引脚
    uint8_t active_level;       // 有效电平：0-低电平有效，1-高电平有效
} button_gpio_config_t;

/**
 * @brief 初始化GPIO按键
 * @param config GPIO按键配置
 * @return HAL_OK 成功，其他失败
 */
HAL_StatusTypeDef button_gpio_init(const button_gpio_config_t *config);

/**
 * @brief 反初始化GPIO按键
 * @param config GPIO按键配置
 * @return HAL_OK 成功，其他失败
 */
HAL_StatusTypeDef button_gpio_deinit(const button_gpio_config_t *config);

/**
 * @brief 获取GPIO按键电平
 * @param config GPIO按键配置
 * @return 按键电平
 */
uint8_t button_gpio_get_key_level(const button_gpio_config_t *config);

#ifdef __cplusplus
}
#endif

#endif /* __BUTTON_GPIO_H */ 
