/*
 * @环境：本程序基于米醋 STM32F407VET6 电控板 及 HAL 库进行编写
 * @接线：
 * @功能：使用编码器进行速度的闭环 PID 控制（左右轮），按键 1 启动 PID 控制，按键 2 和 按键 3 修改左右轮 PID 的目标值
 * @注意：注意要悬空进行测试
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
  PID_Init();
  Uart_Printf(DEBUG_UART, "==== Finish ====\r\n");
  HAL_TIM_Base_Start_IT(&htim2);
}

unsigned char key_timer10ms;
unsigned char encoder_timer10ms; // 编码器采样周期为 10ms
unsigned char gyroscope_timer10ms; // 陀螺仪采样周期为 10ms
unsigned char pid_timer10ms; // PID 计算周期为 10ms

// TIM2 中断服务函数（1ms 中断）
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance != htim2.Instance) return;
  
  if(++key_timer10ms >= 10)
  {
    key_timer10ms = 0;
    Key_Task();
  }
  
  if(++encoder_timer10ms >= 10)
  {
    encoder_timer10ms = 0;
    Encoder_Task();
  }
  
  if(++gyroscope_timer10ms >= 10)
  {
    gyroscope_timer10ms = 0;
    Gyroscope_Task();
  }
  
  if(++pid_timer10ms >= 10)
  {
    pid_timer10ms = 0;
    PID_Task();
  }
  
}
