/*
 * @环境：本程序基于米醋 STM32F407VET6 电控板 及 HAL 库进行编写
 * @接线：使用硬件 I2C2
 * @功能：在 0.91 寸 OLED 上显示灰度传感器的开关量，同时通过串口打印出来
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
  Uart_Printf(DEBUG_UART, "==== System Init ====\r\n");
}
