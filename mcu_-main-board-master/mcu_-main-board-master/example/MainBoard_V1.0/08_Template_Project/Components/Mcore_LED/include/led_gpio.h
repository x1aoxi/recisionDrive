#ifndef MCORE_LED_GPIO_H
#define MCORE_LED_GPIO_H

#include "led.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建GPIO LED实例
 * @param config GPIO LED配置
 * @return LED句柄
 */
led_handle_t led_gpio_create(const led_gpio_config_t *config);

/**
 * @brief 删除GPIO LED实例
 * @param led_handle LED句柄
 * @return HAL状态
 */
HAL_StatusTypeDef led_gpio_delete(led_handle_t led_handle);

/**
 * @brief 设置GPIO LED亮度
 * @param led_handle LED句柄
 * @param brightness 亮度值（0-255，只能是0或255）
 * @return HAL状态
 */
HAL_StatusTypeDef led_gpio_set_brightness(led_handle_t led_handle, uint8_t brightness);

/**
 * @brief 获取GPIO LED亮度
 * @param led_handle LED句柄
 * @return 亮度值（0或255）
 */
uint8_t led_gpio_get_brightness(led_handle_t led_handle);

#ifdef __cplusplus
}
#endif

#endif /* MCORE_LED_GPIO_H */ 
