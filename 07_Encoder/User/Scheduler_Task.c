/**
 * @brief 系统初始化和中断处理
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
    PID_Init();
    Uart_Printf(DEBUG_UART, "==== System Init ====\r\n");
    HAL_TIM_Base_Start_IT(&htim2);
}

static uint8_t timer_10ms = 0;

/**
 * @brief TIM2中断回调 (1ms周期)
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance != htim2.Instance) return;

    // 10ms任务
    if (++timer_10ms >= 10) {
        timer_10ms = 0;
        Encoder_Task();  // 编码器采样
        Motor_Task();    // 电机控制
        PID_Task();      // PID计算
    }
}
