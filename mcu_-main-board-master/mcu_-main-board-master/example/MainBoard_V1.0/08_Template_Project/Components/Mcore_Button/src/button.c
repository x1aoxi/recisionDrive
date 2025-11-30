/**
 * @file button.c
 * @brief STM32按键库核心实现
 */

#include <string.h>
#include <stdlib.h>
#include "button.h"

/* 默认参数定义 */
#define BUTTON_LONG_PRESS_TIME_DEFAULT     2000    // 长按时间默认2秒
#define BUTTON_SHORT_PRESS_TIME_DEFAULT    200     // 短按时间默认200ms
#define BUTTON_SCAN_INTERVAL_MS           10      // 按键扫描间隔10ms
#define MAX_BUTTON_CNT                    8       // 最大支持8个按键
#define MAX_CALLBACK_CNT                  5       // 每个事件最多支持5个回调

/* 按键回调函数结构体 */
typedef struct {
    button_event_t event;           // 事件类型
    button_cb_t cb;                 // 回调函数
    void *usr_data;                 // 用户数据
} button_callback_t;

/* 按键控制块 */
typedef struct button_t {
    uint8_t active;                 // 是否激活
    button_type_t type;             // 按键类型
    uint16_t long_press_time;       // 长按时间
    uint16_t short_press_time;      // 短按时间
    uint32_t hold_cnt;              // 按住计数
    uint32_t click_cnt;             // 点击次数
    uint8_t state;                  // 按键状态
    uint8_t debounce_cnt;           // 消抖计数
    uint8_t repeat_cnt;             // 重复计数
    button_event_t event;           // 当前事件
    uint32_t click_timeout_cnt;     // 添加点击超时计数器
    union {
        button_gpio_config_t gpio;  // GPIO配置
    } config_union;                 // 给联合体添加名称
    button_callback_t callbacks[BUTTON_EVENT_MAX][MAX_CALLBACK_CNT];  // 回调函数数组
} button_t;

/* 按键管理结构体 */
static struct {
    button_t *buttons[MAX_BUTTON_CNT];  // 按键数组
    uint8_t button_cnt;                 // 按键数量
} button_manage;

/* 按键状态定义 */
enum {
    BUTTON_STATE_IDLE = 0,          // 空闲状态
    BUTTON_STATE_DEBOUNCE,          // 消抖状态
    BUTTON_STATE_PRESSED,           // 按下状态
    BUTTON_STATE_RELEASE            // 释放状态
};

/* 私有函数声明 */
static void button_process(button_t *button);
static uint8_t get_button_level(button_t *button);
static void button_event_cb_exec(button_t *button, button_event_t event);

button_handle_t button_create(const button_config_t *config)
{
    if (config == NULL || button_manage.button_cnt >= MAX_BUTTON_CNT) {
        return NULL;
    }

    /* 分配内存 */
    button_t *button = (button_t *)malloc(sizeof(button_t));
    if (button == NULL) {
        return NULL;
    }

    /* 初始化按键结构体 */
    memset(button, 0, sizeof(button_t));
    button->type = config->type;
    button->long_press_time = config->long_press_time ? config->long_press_time : BUTTON_LONG_PRESS_TIME_DEFAULT;
    button->short_press_time = config->short_press_time ? config->short_press_time : BUTTON_SHORT_PRESS_TIME_DEFAULT;
    button->active = 1;

    /* 根据按键类型初始化 */
    HAL_StatusTypeDef ret = HAL_ERROR;
    switch (config->type) {
        case BUTTON_TYPE_GPIO:
            memcpy(&button->config_union.gpio, &config->button_union.gpio_button_config, sizeof(button_gpio_config_t));
            ret = button_gpio_init(&button->config_union.gpio);
            break;
        default:
            free(button);
            return NULL;
    }

    if (ret != HAL_OK) {
        free(button);
        return NULL;
    }

    /* 添加到按键管理数组 */
    button_manage.buttons[button_manage.button_cnt++] = button;

    return (button_handle_t)button;
}

HAL_StatusTypeDef button_delete(button_handle_t btn_handle)
{
    button_t *button = (button_t *)btn_handle;
    if (button == NULL) {
        return HAL_ERROR;
    }

    /* 根据按键类型反初始化 */
    switch (button->type) {
        case BUTTON_TYPE_GPIO:
            button_gpio_deinit(&button->config_union.gpio);
            break;
        default:
            return HAL_ERROR;
    }

    /* 从按键管理数组中移除 */
    for (uint8_t i = 0; i < button_manage.button_cnt; i++) {
        if (button_manage.buttons[i] == button) {
            for (uint8_t j = i; j < button_manage.button_cnt - 1; j++) {
                button_manage.buttons[j] = button_manage.buttons[j + 1];
            }
            button_manage.button_cnt--;
            break;
        }
    }

    /* 释放内存 */
    free(button);

    return HAL_OK;
}

HAL_StatusTypeDef button_register_cb(button_handle_t btn_handle, button_event_t event, button_cb_t cb, void *usr_data)
{
    button_t *button = (button_t *)btn_handle;
    if (button == NULL || event >= BUTTON_EVENT_MAX || cb == NULL) {
        return HAL_ERROR;
    }

    /* 查找空闲的回调槽位 */
    for (uint8_t i = 0; i < MAX_CALLBACK_CNT; i++) {
        if (button->callbacks[event][i].cb == NULL) {
            button->callbacks[event][i].event = event;
            button->callbacks[event][i].cb = cb;
            button->callbacks[event][i].usr_data = usr_data;
            return HAL_OK;
        }
    }

    return HAL_ERROR;
}

HAL_StatusTypeDef button_unregister_cb(button_handle_t btn_handle, button_event_t event)
{
    button_t *button = (button_t *)btn_handle;
    if (button == NULL || event >= BUTTON_EVENT_MAX) {
        return HAL_ERROR;
    }

    /* 清除所有该事件的回调 */
    for (uint8_t i = 0; i < MAX_CALLBACK_CNT; i++) {
        if (button->callbacks[event][i].cb != NULL) {
            memset(&button->callbacks[event][i], 0, sizeof(button_callback_t));
        }
    }

    return HAL_OK;
}

button_event_t button_get_event(button_handle_t btn_handle)
{
    button_t *button = (button_t *)btn_handle;
    if (button == NULL) {
        return BUTTON_NONE_PRESS;
    }

    return button->event;
}

uint8_t button_get_key_level(button_handle_t btn_handle)
{
    button_t *button = (button_t *)btn_handle;
    if (button == NULL) {
        return 0;
    }

    return get_button_level(button);
}

void button_periodic_handler(void)
{
    for (uint8_t i = 0; i < button_manage.button_cnt; i++) {
        if (button_manage.buttons[i]->active) {
            button_process(button_manage.buttons[i]);
        }
    }
}

/* 私有函数实现 */
static uint8_t get_button_level(button_t *button)
{
    uint8_t level = 0;

    switch (button->type) {
        case BUTTON_TYPE_GPIO:
            level = button_gpio_get_key_level(&button->config_union.gpio);
            break;
        default:
            break;
    }

    return level;
}

static void button_event_cb_exec(button_t *button, button_event_t event)
{
    button->event = event;

    for (uint8_t i = 0; i < MAX_CALLBACK_CNT; i++) {
        if (button->callbacks[event][i].cb) {
            button->callbacks[event][i].cb(button, button->callbacks[event][i].usr_data);
        }
    }
}

static void button_process(button_t *button)
{
    uint8_t current_level = get_button_level(button);

    switch (button->state) {
        case BUTTON_STATE_IDLE:
            if (current_level == button->config_union.gpio.active_level) {
                button->state = BUTTON_STATE_DEBOUNCE;
                button->debounce_cnt = 0;
            } else if (button->click_cnt > 0) {
                // 在空闲状态下检查点击超时
                button->click_timeout_cnt++;
                if (button->click_timeout_cnt * BUTTON_SCAN_INTERVAL_MS >= button->short_press_time) {
                    // 超时，触发相应的点击事件
                    if (button->click_cnt == 1) {
                        button_event_cb_exec(button, BUTTON_SINGLE_CLICK);
                    } else if (button->click_cnt == 2) {
                        button_event_cb_exec(button, BUTTON_DOUBLE_CLICK);
                    } else if (button->click_cnt > 2) {
                        button_event_cb_exec(button, BUTTON_MULTIPLE_CLICK);
                    }
                    button->click_cnt = 0;
                    button->click_timeout_cnt = 0;
                }
            }
            break;

        case BUTTON_STATE_DEBOUNCE:
            if (current_level == button->config_union.gpio.active_level) {
                button->debounce_cnt++;
                if (button->debounce_cnt >= 2) {  // 20ms消抖
                    button->state = BUTTON_STATE_PRESSED;
                    button->hold_cnt = 0;
                    button->event = BUTTON_PRESS_DOWN;
                    button_event_cb_exec(button, BUTTON_PRESS_DOWN);
                }
            } else {
                button->state = BUTTON_STATE_IDLE;
            }
            break;

        case BUTTON_STATE_PRESSED:
            if (current_level != button->config_union.gpio.active_level) {
                button->state = BUTTON_STATE_RELEASE;
                button_event_cb_exec(button, BUTTON_PRESS_UP);

                /* 判断按键时间 */
                if (button->hold_cnt * BUTTON_SCAN_INTERVAL_MS < button->short_press_time) {
                    button->click_cnt++;
                    button->click_timeout_cnt = 0;
                } else if (button->hold_cnt * BUTTON_SCAN_INTERVAL_MS >= button->long_press_time) {
                    button_event_cb_exec(button, BUTTON_LONG_PRESS_UP);
                    button->click_cnt = 0;
                }
            } else {
                button->hold_cnt++;

                /* 长按检测 */
                if (button->hold_cnt * BUTTON_SCAN_INTERVAL_MS == button->long_press_time) {
                    button_event_cb_exec(button, BUTTON_LONG_PRESS_START);
                }
                if (button->hold_cnt * BUTTON_SCAN_INTERVAL_MS > button->long_press_time) {
                    button_event_cb_exec(button, BUTTON_LONG_PRESS_HOLD);
                }
            }
            break;

        case BUTTON_STATE_RELEASE:
            if (current_level == button->config_union.gpio.active_level) {
                button->state = BUTTON_STATE_DEBOUNCE;
            } else {
                button->state = BUTTON_STATE_IDLE;
            }
            break;

        default:
            button->state = BUTTON_STATE_IDLE;
            break;
    }
} 
