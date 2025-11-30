/*
 * @环境：本程序基于米醋 STM32F407VET6 电控板 及 HAL 库进行编写
 * @功能：利用 DMA + 空闲中断 + 环形缓冲区实现串口 1 的数据收发，按键按下发送对应的提示
*/

#include "Scheduler_Task.h"

void System_Init(void)
{
	Uart_Tx_Init();
  Led_Init();
  Key_Init();
  Uart_Init();
  Uart_Printf(DEBUG_UART, "==== System Init ====\r\n");
}
