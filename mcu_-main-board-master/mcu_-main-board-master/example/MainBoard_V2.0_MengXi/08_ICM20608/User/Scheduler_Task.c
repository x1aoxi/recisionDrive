/*
 * @环境：本程序基于米醋 STM32F407VET6 电控板 及 HAL 库进行编写
 * @接线：使用硬件 I2C1
 * @功能：串口打印 ICM20608 陀螺仪的六轴原始数据，温度，解算后的欧拉角，并在 OLED 上显示欧拉角
 * @注意：上电要静止直到数据正常显示（校准），如果串口数据不正常请断电重启，而不是复位
*/

#include "Scheduler_Task.h"

void System_Init(void)
{
	Uart_Tx_Init();
  Led_Init();
  Key_Init();
  Uart_Init();
  Oled_Init();
  Gyroscope_Init();
  Uart_Printf(DEBUG_UART, "==== System Init ====\r\n");
  HAL_TIM_Base_Start_IT(&htim2);
}

unsigned char gyroscope_timer10ms; // 陀螺仪采样周期为 10ms

// TIM2 中断服务函数（1ms 中断）
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance != htim2.Instance) return;
  
  if(++gyroscope_timer10ms >= 10)
  {
    gyroscope_timer10ms = 0;
    Gyroscope_Task();
  }
}
