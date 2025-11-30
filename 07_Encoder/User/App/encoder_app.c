#include "encoder_app.h"

// 编码器实例
Encoder left_encoder;
Encoder right_encoder;

/**
 * @brief 初始化编码器应用
 */
void Encoder_Init(void)
{
#if MOTOR_COUNT == 2
    // 初始化左编码器 (TIM3)
    Encoder_Driver_Init(&left_encoder, &htim3, 1);
#endif
    // 初始化右编码器 (TIM4)
    Encoder_Driver_Init(&right_encoder, &htim4, 1);
}

/**
 * @brief 编码器应用运行任务
 */
void Encoder_Task(void)
{
#if MOTOR_COUNT == 2
    // 更新左编码器
    Encoder_Driver_Update(&left_encoder);
#endif
    // 更新右编码器
    Encoder_Driver_Update(&right_encoder);
}
