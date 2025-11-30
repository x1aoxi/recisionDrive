#include "led_pwm.h"
#include "elog.h"
#include <stdlib.h>

static const char *TAG = "LED_PWM";

led_handle_t led_pwm_create(const led_pwm_config_t *config)
{
    led_pwm_t *led = NULL;

    if (config == NULL) {
        elog_e(TAG, "PWM LED config is NULL");
        return NULL;
    }

    led = (led_pwm_t *)malloc(sizeof(led_pwm_t));
    if (led == NULL) {
        elog_e(TAG, "Failed to allocate memory for PWM LED");
        return NULL;
    }

    /* 保存配置 */
    led->type = LED_TYPE_PWM;
    led->config = *config;
    led->current_brightness = 0;

    /* 启动所有配置的PWM通道 */
    if (config->channel & TIM_CHANNEL_1) {
        HAL_TIM_PWM_Start(config->htim, TIM_CHANNEL_1);
        __HAL_TIM_SET_COMPARE(config->htim, TIM_CHANNEL_1, config->period); // 初始状态为LED熄灭（满占空比）
    }
    if (config->channel & TIM_CHANNEL_2) {
        HAL_TIM_PWM_Start(config->htim, TIM_CHANNEL_2);
        __HAL_TIM_SET_COMPARE(config->htim, TIM_CHANNEL_2, config->period);
    }
    if (config->channel & TIM_CHANNEL_3) {
        HAL_TIM_PWM_Start(config->htim, TIM_CHANNEL_3);
        __HAL_TIM_SET_COMPARE(config->htim, TIM_CHANNEL_3, config->period);
    }
    if (config->channel & TIM_CHANNEL_4) {
        HAL_TIM_PWM_Start(config->htim, TIM_CHANNEL_4);
        __HAL_TIM_SET_COMPARE(config->htim, TIM_CHANNEL_4, config->period);
    }

    elog_i(TAG, "PWM LED created");
    return (led_handle_t)led;
}

HAL_StatusTypeDef led_pwm_delete(led_handle_t led_handle)
{
    led_pwm_t *led = (led_pwm_t *)led_handle;

    if (led == NULL) {
        return HAL_ERROR;
    }

    /* 停止所有配置的PWM通道 */
    if (led->config.channel & TIM_CHANNEL_1) {
        __HAL_TIM_SET_COMPARE(led->config.htim, TIM_CHANNEL_1, led->config.period); // 关闭LED
        HAL_TIM_PWM_Stop(led->config.htim, TIM_CHANNEL_1);
    }
    if (led->config.channel & TIM_CHANNEL_2) {
        __HAL_TIM_SET_COMPARE(led->config.htim, TIM_CHANNEL_2, led->config.period);
        HAL_TIM_PWM_Stop(led->config.htim, TIM_CHANNEL_2);
    }
    if (led->config.channel & TIM_CHANNEL_3) {
        __HAL_TIM_SET_COMPARE(led->config.htim, TIM_CHANNEL_3, led->config.period);
        HAL_TIM_PWM_Stop(led->config.htim, TIM_CHANNEL_3);
    }
    if (led->config.channel & TIM_CHANNEL_4) {
        __HAL_TIM_SET_COMPARE(led->config.htim, TIM_CHANNEL_4, led->config.period);
        HAL_TIM_PWM_Stop(led->config.htim, TIM_CHANNEL_4);
    }

    /* 释放内存 */
    free(led);
    return HAL_OK;
}

HAL_StatusTypeDef led_pwm_set_brightness(led_handle_t led_handle, uint8_t brightness)
{
    led_pwm_t *led = (led_pwm_t *)led_handle;
    uint32_t compare_value;

    if (led == NULL) {
        return HAL_ERROR;
    }

    /* 计算PWM比较值（反转逻辑：亮度越大，占空比越小）*/
    compare_value = ((255 - brightness) * led->config.period) / 255;
    
    /* 设置所有配置的PWM通道 */
    if (led->config.channel & TIM_CHANNEL_1) {
        __HAL_TIM_SET_COMPARE(led->config.htim, TIM_CHANNEL_1, compare_value);
    }
    if (led->config.channel & TIM_CHANNEL_2) {
        __HAL_TIM_SET_COMPARE(led->config.htim, TIM_CHANNEL_2, compare_value);
    }
    if (led->config.channel & TIM_CHANNEL_3) {
        __HAL_TIM_SET_COMPARE(led->config.htim, TIM_CHANNEL_3, compare_value);
    }
    if (led->config.channel & TIM_CHANNEL_4) {
        __HAL_TIM_SET_COMPARE(led->config.htim, TIM_CHANNEL_4, compare_value);
    }
    
    led->current_brightness = brightness;
    return HAL_OK;
}

uint8_t led_pwm_get_brightness(led_handle_t led_handle)
{
    led_pwm_t *led = (led_pwm_t *)led_handle;

    if (led == NULL) {
        return 0;
    }

    return led->current_brightness;
} 
