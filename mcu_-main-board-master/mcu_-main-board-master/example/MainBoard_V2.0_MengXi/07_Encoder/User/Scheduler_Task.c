/*
 * @环境：本程序基于米醋 STM32F407VET6 电控板 及 HAL 库进行编写
 * @功能：在 0.91 寸 OLED 上显示电机的转速(rmp 和 cm/s)，同时用串口打印出来
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
  Encoder_Init();
  Uart_Printf(DEBUG_UART, "==== System Init ====\r\n");
  HAL_TIM_Base_Start_IT(&htim2);
}

unsigned char encoder_timer10ms;

// TIM2 中断服务函数（1ms 中断）
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance != htim2.Instance) return;
  
  if(++encoder_timer10ms >= 10)
  {
    encoder_timer10ms = 0;
    Encoder_Task();
  }
  
}
