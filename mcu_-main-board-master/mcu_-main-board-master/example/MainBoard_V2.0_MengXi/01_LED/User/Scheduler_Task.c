/*
 * @环境：本程序基于米醋 STM32F407VET6 电控板 及 HAL 库进行编写
 * @功能：上电 LED 按照 1010 的顺序显示
*/

#include "Scheduler_Task.h"

void System_Init(void)
{
  Led_Init();
}
