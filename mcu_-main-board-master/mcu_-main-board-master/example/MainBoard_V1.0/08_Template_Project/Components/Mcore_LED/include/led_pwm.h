#ifndef MCORE_LED_PWM_H
#define MCORE_LED_PWM_H

#include "led.h"

#ifdef __cplusplus
extern "C" {
#endif

/* PWM LED结构体 */
typedef struct {
    led_type_t type;
    led_pwm_config_t config;
    uint8_t current_brightness;
} led_pwm_t;

/**
 * @brief 创建PWM LED实例
 * @param config PWM LED配置
 * @return LED句柄
 */
led_handle_t led_pwm_create(const led_pwm_config_t *config);

/**
 * @brief 删除PWM LED实例
 * @param led_handle LED句柄
 * @return HAL状态
 */
HAL_StatusTypeDef led_pwm_delete(led_handle_t led_handle);

/**
 * @brief 设置PWM LED亮度
 * @param led_handle LED句柄
 * @param brightness 亮度值（0-255）
 * @return HAL状态
 */
HAL_StatusTypeDef led_pwm_set_brightness(led_handle_t led_handle, uint8_t brightness);

/**
 * @brief 获取PWM LED亮度
 * @param led_handle LED句柄
 * @return 亮度值（0-255）
 */
uint8_t led_pwm_get_brightness(led_handle_t led_handle);

#ifdef __cplusplus
}
#endif

#endif /* MCORE_LED_PWM_H */ 
