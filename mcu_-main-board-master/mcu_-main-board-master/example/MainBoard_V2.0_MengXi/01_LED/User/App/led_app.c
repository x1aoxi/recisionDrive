#include "led_app.h"

uint8_t led_buf[4] = {1, 0, 1, 0};

void Led_Init()
{
    Led_Display(led_buf, 0);
}

void Led_Task()
{
    Led_Display(led_buf, 1);
}
