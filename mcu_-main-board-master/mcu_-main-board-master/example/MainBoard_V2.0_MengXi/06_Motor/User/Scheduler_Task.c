/*
 * @环境：本程序基于米醋 STM32F407VET6 电控板 及 HAL 库进行编写
 * @接线：使用 TIM1 生成 PWM，要注意 GND 和 VCC 的位置，大概率要反向的线
 * @功能：在 0.91 寸 OLED 上显示电机的占空比以及方向（正负），长按对应按键增加或减小
*/

#include "Scheduler_Task.h"

void System_Init(void)
{
	Uart_Tx_Init();
  Led_Init();
  Key_Init();
  Uart_Init();
  Oled_Init();
  Gray_Init();
  Motor_Init();
  Uart_Printf(DEBUG_UART, "==== System Init ====\r\n");
}
