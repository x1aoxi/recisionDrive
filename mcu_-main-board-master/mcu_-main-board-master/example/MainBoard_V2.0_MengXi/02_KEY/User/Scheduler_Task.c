/*
 * @环境：本程序基于米醋 STM32F407VET6 电控板 及 HAL 库进行编写
 * @功能：双击点亮，单击熄灭，长按闪烁
*/

#include "Scheduler_Task.h"

void System_Init(void)
{
  Led_Init();
  Key_Init();
}
