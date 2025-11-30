#ifndef MCORE_LED_H
#define MCORE_LED_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/* LED类型定义 */
typedef enum {
    LED_TYPE_GPIO = 0,    // GPIO控制LED
    LED_TYPE_PWM,         // PWM控制LED
} led_type_t;

/* LED句柄 */
typedef void *led_handle_t;

/* GPIO LED配置 */
typedef struct {
    GPIO_TypeDef *gpio_port;  // GPIO端口
    uint16_t gpio_pin;        // GPIO引脚
    uint8_t active_level;     // 有效电平（0：低电平有效，1：高电平有效）
} led_gpio_config_t;

/* PWM LED配置 */
typedef struct {
    TIM_HandleTypeDef *htim;  // 定时器句柄
    uint32_t channel;         // 定时器通道
    uint32_t period;          // PWM周期
} led_pwm_config_t;

/* LED配置联合体 */
typedef union {
    led_gpio_config_t gpio_led_config;
    led_pwm_config_t pwm_led_config;
} led_config_union_t;

/* LED配置结构体 */
typedef struct {
    led_type_t type;                 // LED类型
    led_config_union_t led_union;    // LED配置联合体
} led_config_t;

/**
 * @brief 创建LED实例
 * @param config LED配置
 * @return LED句柄
 */
led_handle_t led_create(const led_config_t *config);

/**
 * @brief 删除LED实例
 * @param led_handle LED句柄
 * @return HAL状态
 */
HAL_StatusTypeDef led_delete(led_handle_t led_handle);

/**
 * @brief 设置LED亮度
 * @param led_handle LED句柄
 * @param brightness 亮度值（0-255）
 * @return HAL状态
 */
HAL_StatusTypeDef led_set_brightness(led_handle_t led_handle, uint8_t brightness);

/**
 * @brief 获取LED亮度
 * @param led_handle LED句柄
 * @return 亮度值（0-255）
 */
uint8_t led_get_brightness(led_handle_t led_handle);

#ifdef __cplusplus
}
#endif

#endif /* MCORE_LED_H */ 
