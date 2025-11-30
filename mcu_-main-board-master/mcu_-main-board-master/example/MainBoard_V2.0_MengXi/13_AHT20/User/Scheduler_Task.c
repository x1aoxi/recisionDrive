/*
 * @环境：本程序基于米醋 STM32F407VET6 电控板 及 HAL 库进行编写
 * @接线：
 * @功能：使用 Oled 和串口显示 AHT20 的温度和湿度数据
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
  Aht20_Init();
  Uart_Printf(DEBUG_UART, "==== Finish ====\r\n");
  HAL_TIM_Base_Start_IT(&htim2);
}

unsigned char key_timer10ms = 0;
unsigned char gyroscope_timer10ms = 0;

// TIM2 中断服务函数（1ms 中断）
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance != htim2.Instance) return;

  /* 按键扫描任务 */
  if(++key_timer10ms >= 10)
  {
    key_timer10ms = 0;
    Key_Task();
  }
  
  if(++gyroscope_timer10ms >= 10)
  {
    gyroscope_timer10ms = 0;
    Gyroscope_Task();
  }
}
