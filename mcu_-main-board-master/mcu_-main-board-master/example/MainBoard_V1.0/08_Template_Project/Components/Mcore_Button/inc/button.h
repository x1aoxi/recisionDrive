/**
 * @file button.h
 * @brief STM32按键库头文件
 */

#ifndef __BUTTON_H
#define __BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "button_gpio.h"

typedef void (* button_cb_t)(void *button_handle, void *usr_data);
typedef void *button_handle_t;

/**
 * @brief 按键事件类型定义
 */
typedef enum {
    BUTTON_PRESS_DOWN = 0,      // 按下事件
    BUTTON_PRESS_UP,           // 释放事件
    BUTTON_PRESS_REPEAT,       // 重复按下事件
    BUTTON_PRESS_REPEAT_DONE,  // 重复按下完成
    BUTTON_SINGLE_CLICK,       // 单击事件
    BUTTON_DOUBLE_CLICK,       // 双击事件
    BUTTON_MULTIPLE_CLICK,     // 多击事件
    BUTTON_LONG_PRESS_START,   // 长按开始
    BUTTON_LONG_PRESS_HOLD,    // 长按保持
    BUTTON_LONG_PRESS_UP,      // 长按释放
    BUTTON_PRESS_END,          // 按键事件结束
    BUTTON_EVENT_MAX,
    BUTTON_NONE_PRESS,
} button_event_t;

/**
 * @brief 按键事件数据
 */
typedef union {
    struct {
        uint16_t press_time;    // 按压时间(ms)
    } long_press;
    
    struct {
        uint16_t clicks;        // 点击次数
    } multiple_clicks;
} button_event_data_t;

/**
 * @brief 按键事件配置
 */
typedef struct {
    button_event_t event;
    button_event_data_t event_data;
} button_event_config_t;

/**
 * @brief 按键类型
 */
typedef enum {
    BUTTON_TYPE_GPIO,
    BUTTON_TYPE_CUSTOM
} button_type_t;

/**
 * @brief 按键参数
 */
typedef enum {
    BUTTON_LONG_PRESS_TIME_MS = 0,
    BUTTON_SHORT_PRESS_TIME_MS,
    BUTTON_PARAM_MAX,
} button_param_t;

/**
 * @brief 按键配置
 */
typedef struct {
    button_type_t type;
    uint16_t long_press_time;
    uint16_t short_press_time;
    union {
        button_gpio_config_t gpio_button_config;
    } button_union;
} button_config_t;

/**
 * @brief 创建按键实例
 * @param config 按键配置
 * @return 按键句柄
 */
button_handle_t button_create(const button_config_t *config);

/**
 * @brief 删除按键实例
 * @param btn_handle 按键句柄
 * @return HAL_OK 成功，其他失败
 */
HAL_StatusTypeDef button_delete(button_handle_t btn_handle);

/**
 * @brief 注册按键事件回调函数
 * @param btn_handle 按键句柄
 * @param event 事件类型
 * @param cb 回调函数
 * @param usr_data 用户数据
 * @return HAL_OK 成功，其他失败
 */
HAL_StatusTypeDef button_register_cb(button_handle_t btn_handle, button_event_t event, button_cb_t cb, void *usr_data);

/**
 * @brief 注销按键事件回调函数
 * @param btn_handle 按键句柄
 * @param event 事件类型
 * @return HAL_OK 成功，其他失败
 */
HAL_StatusTypeDef button_unregister_cb(button_handle_t btn_handle, button_event_t event);

/**
 * @brief 获取按键当前状态
 * @param btn_handle 按键句柄
 * @return 按键事件类型
 */
button_event_t button_get_event(button_handle_t btn_handle);

/**
 * @brief 获取按键电平
 * @param btn_handle 按键句柄
 * @return 按键电平
 */
uint8_t button_get_key_level(button_handle_t btn_handle);

/**
 * @brief 按键定时处理函数，需要在定时器中断中调用
 */
void button_periodic_handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __BUTTON_H */ 
