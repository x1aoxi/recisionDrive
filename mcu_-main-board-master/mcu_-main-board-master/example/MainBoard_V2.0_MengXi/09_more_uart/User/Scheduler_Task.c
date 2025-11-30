/*
 * @环境：本程序基于米醋 STM32F407VET6 电控板 及 HAL 库进行编写
 * @接线：
 * @功能：串口 1，串口 5，串口 6 实现数据收发
 * @注意：
*/

#include "Scheduler_Task.h"

void System_Init(void)
{
	Uart_Tx_Init();
  Uart_Printf(DEBUG_UART, "==== System Init ====\r\n");
  Led_Init();
  Key_Init();
  Uart_Init();
  Oled_Init();
  Gray_Init();
  Motor_Init();
  Encoder_Init();
  Gyroscope_Init();
  Uart_Printf(DEBUG_UART, "==== Finish ====\r\n");
  HAL_TIM_Base_Start_IT(&htim2);
}

unsigned char encoder_timer10ms; // 编码器采样周期为 10ms
unsigned char gyroscope_timer10ms; // 陀螺仪采样周期为 10ms

// TIM2 中断服务函数（1ms 中断）
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance != htim2.Instance) return;
}
