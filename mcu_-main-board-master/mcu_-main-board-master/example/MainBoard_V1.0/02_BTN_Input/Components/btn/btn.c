#include "btn.h"

button btns[BTN_MAX_NUM];
uint8_t btn_count = 0;

__weak void Btn_0_handler(button *btn)
{
    switch (btn->btn_state) {
        case BTN_STA_IDLE:
            break;
        case BTN_STA_LONG_PRESS:
            break;
        case BTN_STA_CLICK:
            break;
        case BTN_STA_DOUBLE_CLICK:
            break;
    }
}

void button_handler(button *btn)
{
    uint8_t gpio_level = HAL_GPIO_ReadPin(btn->GPIOx, btn->GPIO_Pin);

    if (btn->state > 0)
        btn->ticks++;

    if (btn->level != gpio_level) {
        if (++(btn->debouce_cnt) >= 3) {
            btn->level = gpio_level;
            btn->debouce_cnt = 0;
        }
    } else {
        btn->debouce_cnt = 0;
    }

    switch (btn->state) {
        case 0:
            if (btn->level == 0) {
                btn->state = 1;
                btn->ticks = 0;
                btn->repeat = 1;
            } else {
                btn->btn_state = BTN_STA_IDLE;
            }
            break;
        case 1:
            if (btn->level != 0) {
                btn->ticks = 0;
                btn->state = 2;
            } else if (btn->ticks >= 50) {
                btn->btn_state = BTN_STA_LONG_PRESS;
                btn->button_handler(btn);
                btn->ticks = 0;
                btn->repeat = 0;
            }
            break;
        case 2:
            if (btn->ticks > 15) {
                if (btn->repeat == 1) {
                    btn->btn_state = BTN_STA_CLICK;
                    btn->button_handler(btn);
                } else if (btn->repeat == 2) {
                    btn->btn_state = BTN_STA_DOUBLE_CLICK;
                    btn->button_handler(btn);
                }
                btn->state = 0;
            } else {
                if (btn->level == 0) {
                    btn->repeat++;
                    btn->state = 1;
                    btn->ticks = 0;
                }
            }
            break;
    }
}

/**
 * @brief  注册一个按键
 * @param  GPIOx: GPIO端口
 * @param  GPIO_Pin: GPIO引脚
 * @param  button_handler: 按键处理回调函数
 * @return int: 0-成功，-1-失败（按键数量超出最大值）
 */
int Btn_Register(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, void (*handler)(button *))
{
    if (btn_count >= BTN_MAX_NUM) {
        return -1;  // 按键数量超出最大值
    }

    btns[btn_count].GPIOx = GPIOx;
    btns[btn_count].GPIO_Pin = GPIO_Pin;
    btns[btn_count].level = 1;
    btns[btn_count].id = btn_count;
    btns[btn_count].button_handler = handler;
    btns[btn_count].state = 0;
    btns[btn_count].ticks = 0;
    btns[btn_count].repeat = 0;
    btns[btn_count].debouce_cnt = 0;
    btns[btn_count].btn_state = BTN_STA_IDLE;

    btn_count++;
    return 0;
}

void Key_Proc(void)
{
    for (uint8_t i = 0; i < btn_count; i++) {
        button_handler(&btns[i]);
    }
}

void Key_Init(void)
{
    btn_count = 0;
    Btn_Register(GPIOC, GPIO_PIN_3, &Btn_0_handler);
}
