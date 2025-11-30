/*
 * @环境：本程序基于米醋 STM32F407VET6 电控板 及 HAL 库进行编写
 * @接线：
 * @功能：按下按键 1 两电机一起向 CW 方向移动，按下按键 2 两电机一起向 CCW 方向移动，按下按键 3 停止
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
  ZDT_Motor_Init();
  PID_Init();
  Uart_Printf(DEBUG_UART, "==== Finish ====\r\n");
  HAL_TIM_Base_Start_IT(&htim2);
}

// TIM2 中断服务函数（1ms 中断）
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance != htim2.Instance) return;

  
}
