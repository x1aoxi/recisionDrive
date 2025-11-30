#include "pid_app.h"

#if MOTOR_COUNT == 2
extern Encoder left_encoder;
#endif
extern Encoder right_encoder;

#if MOTOR_COUNT == 2
extern MOTOR left_motor;
#endif
extern MOTOR right_motor;

int basic_speed = 40;

/* PID控制器 */
PID_T pid_speed_left;
PID_T pid_speed_right;

#if MOTOR_COUNT == 2
PidParams_t pid_params_left = {
    .kp = 33.0f,
    .ki = 4.5f,
    .kd = 0.0f,
    .out_min = -999.0f,
    .out_max = 999.0f,
};
#endif

PidParams_t pid_params_right = {
    .kp = 33.0f,
    .ki = 4.5f,
    .kd = 0.0f,
    .out_min = -999.0f,
    .out_max = 999.0f,
};

void PID_Init(void)
{
#if MOTOR_COUNT == 2
    pid_init(&pid_speed_left,
             pid_params_left.kp, pid_params_left.ki, pid_params_left.kd,
             0.0f, pid_params_left.out_max);
    pid_set_target(&pid_speed_left, basic_speed);
#endif

    pid_init(&pid_speed_right,
             pid_params_right.kp, pid_params_right.ki, pid_params_right.kd,
             0.0f, pid_params_right.out_max);
    pid_set_target(&pid_speed_right, basic_speed);
}

unsigned char pid_running = 0;

void PID_Task(void)
{
    if (pid_running == 0) return;

#if MOTOR_COUNT == 2
    int output_left = pid_calculate_positional(&pid_speed_left, left_encoder.rpm);
    Motor_Set_Speed(&left_motor, output_left);
#endif

    int output_right = pid_calculate_positional(&pid_speed_right, right_encoder.rpm);
    Motor_Set_Speed(&right_motor, output_right);
}

