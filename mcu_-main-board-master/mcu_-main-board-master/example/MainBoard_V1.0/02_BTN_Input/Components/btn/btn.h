#ifndef BTN_H
#define BTN_H

#include "main.h"

/* 定义最大支持的按键数量 */
#define BTN_MAX_NUM  16

typedef enum {
    BTN_STA_IDLE,
    BTN_STA_LONG_PRESS,
    BTN_STA_CLICK,
    BTN_STA_DOUBLE_CLICK,
} button_state_t;

typedef struct btn {
    uint16_t ticks;
    uint8_t level;
    uint8_t id;
    uint8_t state;
    uint8_t repeat;
    uint8_t debouce_cnt;
    
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;

    button_state_t btn_state;
    void (*button_handler)(struct btn *btn);
} button;

/**
 * @brief  按键0的默认处理函数（弱定义）
 * @param  btn: 按键结构体指针
 * @retval None
 */
void Btn_0_handler(button *btn);

void Key_Init(void);
void Key_Proc(void);

/**
 * @brief  注册一个按键
 * @param  GPIOx: GPIO端口
 * @param  GPIO_Pin: GPIO引脚
 * @param  button_handler: 按键处理回调函数
 * @return int: 0-成功，-1-失败（按键数量超出最大值）
 */
int Btn_Register(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, void (*handler)(button *));

#endif /* BTN_H */
