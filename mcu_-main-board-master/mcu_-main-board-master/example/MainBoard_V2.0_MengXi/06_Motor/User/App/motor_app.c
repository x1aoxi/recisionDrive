#include "motor_app.h"

MOTOR left_motor;
MOTOR right_motor;

void Motor_Init(void)
{
    Motor_Config_Init(&left_motor, &htim1, TIM_CHANNEL_2, &htim1, TIM_CHANNEL_1, 1, 500);
    Motor_Config_Init(&right_motor, &htim1, TIM_CHANNEL_4, &htim1, TIM_CHANNEL_3, 0, 500);
}

void Motor_Task(void)
{
    Motor_Set_Speed(&right_motor, 550);
}
