#include "motor_app.h"
#include "pid_app.h"
#include "encoder_app.h"

// ============================= 外部变量引用 =============================
extern Encoder left_encoder;
extern Encoder right_encoder;
extern PID_T pid_speed_left;
extern PID_T pid_speed_right;
extern unsigned char pid_running;

// 选择用于圈数控制的编码器 (只有右轮电机,使用右轮编码器)
#define CIRCLE_CONTROL_ENCODER  right_encoder

// ============================= 电机硬件实例 =============================
#if MOTOR_COUNT == 2
MOTOR left_motor;   // 左电机
#endif
MOTOR right_motor;  // 右电机

// ============================= 电机状态 =============================
static MotorState motor_state = {
    .mode = MOTOR_MODE_IDLE,
    .is_running = 0,
    .direction = MOTOR_DIR_FORWARD,
    .basic_speed = 40.0f,
    .current_gear = SPEED_GEAR_LOW,
    .target_rpm = 30.0f,
    .accel_mode = ACCEL_MODE_LOW,
    .accel_target_rpm = 30.0f,
    .trapezoid_phase = TRAPEZOID_IDLE,
    .trapezoid_timer = 0,
    .trapezoid_current_rpm = 30.0f,
    .circle_state = CIRCLE_IDLE,
    .target_circles = 5,
    .start_total_count = 0,
    .current_circles = 0.0f,
    .remain_circles = 0.0f,
    .current_rpm = 0.0f,
    .right_rpm = 0.0f
};

// ============================= PWM配置表(集中管理) =============================

/**
 * @brief PWM配置表 - 所有参数集中管理
 */
typedef struct {
    // 基本运行模式
    int basic_run_pwm;           // 正转PWM值
    int basic_run_reverse_pwm;   // 反转PWM值

    // 三档变速模式
    int gear_low_pwm;            // 低速档 (30rpm)
    int gear_mid_pwm;            // 中速档 (50rpm)
    int gear_high_pwm;           // 高速档 (80rpm)

    // 加速度测试模式
    float accel_start_rpm;       // 起始转速
    float accel_max_rpm;         // 最大转速
    float accel_rate_low;        // 低加速度 (rpm/s)
    float accel_rate_high;       // 高加速度 (rpm/s)

    // 梯形曲线模式
    float trapezoid_start_rpm;   // 起始转速
    float trapezoid_max_rpm;     // 最大转速
    float trapezoid_accel_rate;  // 加速度 (rpm/s)
    float trapezoid_decel_rate;  // 减速度 (rpm/s)
    uint16_t trapezoid_accel_time_ms;  // 加速时间(ms)
    uint16_t trapezoid_const_time_ms;  // 恒速时间(ms)
    uint16_t trapezoid_decel_time_ms;  // 减速时间(ms)

    // 圈数控制模式
    float circle_control_rpm;    // 运行转速
    int16_t encoder_ppr;         // 编码器PPR
} MotorPWMConfig;

/**
 * @brief PWM配置参数 (根据实测校准)
 * @note 线性关系: PWM = 1.30 * rpm + 529.2
 */
static const MotorPWMConfig pwm_config = {
    // 基本运行 (30rpm)
    .basic_run_pwm = 568,
    .basic_run_reverse_pwm = 568,

    // 三档变速
    .gear_low_pwm = 568,   // 30rpm
    .gear_mid_pwm = 595,   // 50rpm
    .gear_high_pwm = 634,  // 80rpm

    // 加速度测试 (第三问)
    .accel_start_rpm = 16.0f,
    .accel_max_rpm = 200.0f,
    .accel_rate_low = 5.0f,    // 5 rpm/s
    .accel_rate_high = 20.0f,  // 20 rpm/s

    // 梯形曲线 (第四问)
    .trapezoid_start_rpm = 15.0f,
    .trapezoid_max_rpm = 115.0f,
    .trapezoid_accel_rate = 20.0f,
    .trapezoid_decel_rate = 20.0f,
    .trapezoid_accel_time_ms = 5000,
    .trapezoid_const_time_ms = 10000,
    .trapezoid_decel_time_ms = 5000,

    // 圈数控制
    .circle_control_rpm = 16.0f,
    .encoder_ppr = 1551
};

// 三档转速定义(仅用于显示) - 根据实测PWM校准
static const float gear_speeds[] = {30.0f, 50.0f, 80.0f};

// ============================= 初始化函数 =============================

/**
 * @brief 初始化电机硬件
 */
void Motor_Init(void)
{
#if MOTOR_COUNT == 2
    // 初始化左电机 (TIM2)
    Motor_Config_Init(&left_motor, &htim2, TIM_CHANNEL_3, &htim2, TIM_CHANNEL_4, 0, 550);
#endif
    // 初始化右电机 (TIM1)
    Motor_Config_Init(&right_motor, &htim1, TIM_CHANNEL_4, &htim1, TIM_CHANNEL_3, 0, 550);
}

// ============================= 内部辅助函数 =============================

/**
 * @brief 更新实时反馈数据
 */
static void Motor_UpdateFeedback(void)
{
#if MOTOR_COUNT == 2
    motor_state.left_rpm = left_encoder.rpm;
#endif
    motor_state.right_rpm = right_encoder.rpm;

#if MOTOR_COUNT == 2
    // 双电机:取平均值
    motor_state.current_rpm = (left_encoder.rpm_filtered + right_encoder.rpm_filtered) * 0.5f;
#else
    // 单电机:仅用右电机
    motor_state.current_rpm = right_encoder.rpm_filtered;
#endif
}

/**
 * @brief 更新PID目标值 (当前使用开环控制,保留接口)
 */
static void Motor_UpdatePIDTarget(float target_rpm)
{
    (void)target_rpm;
}

/**
 * @brief RPM转换为PWM值
 * @param rpm 目标转速
 * @return PWM值
 * @note 线性关系: 1.30 PWM/rpm
 *       基准点: PWM=550 对应 RPM=16
 *       公式: PWM = 550 + (rpm - 16) * 1.30 = 1.30 * rpm + 529.2
 */
static int Motor_RPM_to_PWM(float rpm)
{
    int pwm = (int)(1.30f * rpm + 529.2f);

    // PWM限幅 (最小100, 最大999)
    if (pwm < 100) pwm = 100;
    if (pwm > 999) pwm = 999;

    return pwm;
}

/**
 * @brief 集中化的电机PWM设置函数
 * @param pwm_value PWM值(正数=正转,负数=反转)
 */
static void Motor_SetPWM(int pwm_value)
{
#if MOTOR_COUNT == 2
    Motor_Set_Speed(&left_motor, pwm_value);
#endif
    Motor_Set_Speed(&right_motor, pwm_value);
}

/**
 * @brief 根据当前模式获取对应的PWM值
 * @return PWM值
 * @note 查表获取,避免硬编码PWM值散落在多处
 */
static int Motor_GetCurrentModePWM(void)
{
    int pwm_value = 0;

    switch (motor_state.mode) {
        case MOTOR_MODE_BASIC_RUN:
            // 根据方向选择对应的PWM值
            if (motor_state.direction == MOTOR_DIR_REVERSE) {
                pwm_value = -pwm_config.basic_run_reverse_pwm;  // 反转使用独立PWM
            } else {
                pwm_value = pwm_config.basic_run_pwm;  // 正转PWM
            }
            break;

        case MOTOR_MODE_SPEED_GEAR:
            // 根据档位查表
            switch (motor_state.current_gear) {
                case SPEED_GEAR_LOW:
                    pwm_value = pwm_config.gear_low_pwm;
                    break;
                case SPEED_GEAR_MID:
                    pwm_value = pwm_config.gear_mid_pwm;
                    break;
                case SPEED_GEAR_HIGH:
                    pwm_value = pwm_config.gear_high_pwm;
                    break;
            }
            break;

        case MOTOR_MODE_ACCELERATION:
            // 加速模式使用实时计算的目标转速
            pwm_value = Motor_RPM_to_PWM(motor_state.accel_target_rpm);
            break;

        case MOTOR_MODE_TRAPEZOID:
            // 梯形模式使用实时计算的目标转速
            pwm_value = Motor_RPM_to_PWM(motor_state.trapezoid_current_rpm);
            break;

        case MOTOR_MODE_CIRCLE_CONTROL:
            // 圈数控制模式使用固定转速
            pwm_value = Motor_RPM_to_PWM(pwm_config.circle_control_rpm);
            break;

        default:
            pwm_value = 0;
            break;
    }

    return pwm_value;
}

// ============================= 任务函数 =============================

/**
 * @brief 电机控制任务(每10ms调用一次)
 * @note 更新实时反馈数据,执行各模式的状态机逻辑
 */
void Motor_Task(void)
{
    // 更新反馈数据
    Motor_UpdateFeedback();

    // 如果未运行,直接返回
    if (!motor_state.is_running) {
        return;
    }

    // 根据当前模式执行特定逻辑
    switch (motor_state.mode) {
        case MOTOR_MODE_IDLE:
        case MOTOR_MODE_BASIC_RUN:
        case MOTOR_MODE_SPEED_GEAR:
            // 这些模式没有动态逻辑,PWM已在Start时设置
            break;

        case MOTOR_MODE_ACCELERATION:
            // 加速度测试模式: 持续加速
            {
                // 获取当前加速度
                float accel_rate = (motor_state.accel_mode == ACCEL_MODE_LOW) ?
                                   pwm_config.accel_rate_low : pwm_config.accel_rate_high;

                // 计算每10ms的增量
                float delta_rpm = accel_rate * 0.01f;
                motor_state.accel_target_rpm += delta_rpm;

                // 限制最大转速
                if (motor_state.accel_target_rpm > pwm_config.accel_max_rpm) {
                    motor_state.accel_target_rpm = pwm_config.accel_max_rpm;
                }

                // 更新PWM
                Motor_SetPWM(Motor_GetCurrentModePWM());
            }
            break;

        case MOTOR_MODE_TRAPEZOID:
            // 梯形曲线模式: 加速 → 恒速 → 减速
            {
                motor_state.trapezoid_timer += 10;  // 递增10ms

                switch (motor_state.trapezoid_phase) {
                    case TRAPEZOID_ACCEL:
                        // 加速阶段
                        {
                            float delta_rpm = pwm_config.trapezoid_accel_rate * 0.01f;
                            motor_state.trapezoid_current_rpm += delta_rpm;

                            // 检查是否达到最大转速或超时
                            if (motor_state.trapezoid_current_rpm >= pwm_config.trapezoid_max_rpm ||
                                motor_state.trapezoid_timer >= pwm_config.trapezoid_accel_time_ms) {
                                motor_state.trapezoid_current_rpm = pwm_config.trapezoid_max_rpm;
                                motor_state.trapezoid_phase = TRAPEZOID_CONST;
                                motor_state.trapezoid_timer = 0;
                            }

                            Motor_SetPWM(Motor_GetCurrentModePWM());
                        }
                        break;

                    case TRAPEZOID_CONST:
                        // 恒速阶段
                        if (motor_state.trapezoid_timer >= pwm_config.trapezoid_const_time_ms) {
                            motor_state.trapezoid_phase = TRAPEZOID_DECEL;
                            motor_state.trapezoid_timer = 0;
                        }
                        // 保持最大转速(PWM已设置)
                        break;

                    case TRAPEZOID_DECEL:
                        // 减速阶段
                        {
                            float delta_rpm = pwm_config.trapezoid_decel_rate * 0.01f;
                            motor_state.trapezoid_current_rpm -= delta_rpm;

                            // 检查是否降到起始转速或超时
                            if (motor_state.trapezoid_current_rpm <= pwm_config.trapezoid_start_rpm ||
                                motor_state.trapezoid_timer >= pwm_config.trapezoid_decel_time_ms) {
                                motor_state.trapezoid_current_rpm = pwm_config.trapezoid_start_rpm;
                                motor_state.trapezoid_phase = TRAPEZOID_FINISHED;
                                motor_state.trapezoid_timer = 0;
                                MotorApp_Stop();  // 自动停止
                                return;  // 已停止,退出
                            }

                            if (motor_state.is_running) {  // 检查是否被Stop中断
                                Motor_SetPWM(Motor_GetCurrentModePWM());
                            }
                        }
                        break;

                    default:
                        break;
                }
            }
            break;

        case MOTOR_MODE_CIRCLE_CONTROL:
            // 圈数控制模式: 监控编码器脉冲数
            if (motor_state.circle_state == CIRCLE_RUNNING) {
                // 计算当前已转脉冲数
                int32_t delta_count = CIRCLE_CONTROL_ENCODER.total_count - motor_state.start_total_count;

                // 处理正反转(取绝对值)
                if (delta_count < 0) delta_count = -delta_count;

                // 计算当前圈数和剩余圈数(用于显示)
                motor_state.current_circles = (float)delta_count / pwm_config.encoder_ppr;
                motor_state.remain_circles = motor_state.target_circles - motor_state.current_circles;
                if (motor_state.remain_circles < 0.0f) {
                    motor_state.remain_circles = 0.0f;
                }

                // 检查是否完成目标圈数
                int32_t target_pulses = motor_state.target_circles * pwm_config.encoder_ppr;
                if (delta_count >= target_pulses) {
                    motor_state.circle_state = CIRCLE_FINISHED;
                    MotorApp_Stop();
                }
            }
            break;

        default:
            break;
    }
}

// ============================= 模式控制接口 =============================

/**
 * @brief 设置运动模式
 * @param mode 目标模式
 */
void MotorApp_SetMode(MotorMode mode)
{
    // 切换模式前先停止电机
    if (motor_state.is_running) {
        MotorApp_Stop();
    }

    motor_state.mode = mode;
}

/**
 * @brief 启动电机
 */
void MotorApp_Start(void)
{
    if (motor_state.is_running) return;

    motor_state.is_running = 1;

    // 根据模式初始化状态
    switch (motor_state.mode) {
        case MOTOR_MODE_ACCELERATION:
            // 加速度模式: 从起始转速开始
            motor_state.accel_target_rpm = pwm_config.accel_start_rpm;
            break;

        case MOTOR_MODE_TRAPEZOID:
            // 梯形曲线模式: 初始化状态机
            motor_state.trapezoid_phase = TRAPEZOID_ACCEL;
            motor_state.trapezoid_timer = 0;
            motor_state.trapezoid_current_rpm = pwm_config.trapezoid_start_rpm;
            break;

        case MOTOR_MODE_CIRCLE_CONTROL:
            // 圈数控制模式: 记录编码器起始脉冲数
            motor_state.circle_state = CIRCLE_RUNNING;
            motor_state.start_total_count = CIRCLE_CONTROL_ENCODER.total_count;
            motor_state.current_circles = 0.0f;
            motor_state.remain_circles = motor_state.target_circles;
            break;

        default:
            // 其他模式无需特殊初始化
            break;
    }

    // 统一设置PWM(通过集中化函数)
    Motor_SetPWM(Motor_GetCurrentModePWM());
}

/**
 * @brief 停止电机
 */
void MotorApp_Stop(void)
{
    if (!motor_state.is_running) return;

    motor_state.is_running = 0;
    Motor_SetPWM(0);
    motor_state.current_rpm = 0.0f;

    // 重置状态机
    if (motor_state.mode == MOTOR_MODE_TRAPEZOID) {
        motor_state.trapezoid_phase = TRAPEZOID_IDLE;
        motor_state.trapezoid_timer = 0;
    }
    if (motor_state.mode == MOTOR_MODE_CIRCLE_CONTROL) {
        motor_state.circle_state = CIRCLE_IDLE;
    }
}

// ============================= Basic Run 模式接口 =============================

/**
 * @brief 设置运动方向
 * @param dir 目标方向
 */
void MotorApp_BasicRun_SetDirection(MotorDirection dir)
{
    motor_state.direction = dir;

    // 如果正在运行,立即更新PWM
    if (motor_state.is_running && motor_state.mode == MOTOR_MODE_BASIC_RUN) {
        Motor_SetPWM(Motor_GetCurrentModePWM());
    }
}

/**
 * @brief 设置基础速度
 * @param speed_rpm 目标速度(rpm)
 */
void MotorApp_BasicRun_SetSpeed(float speed_rpm)
{
    motor_state.basic_speed = speed_rpm;

    // 如果正在运行,立即更新PID目标
    if (motor_state.is_running && motor_state.mode == MOTOR_MODE_BASIC_RUN) {
        float target = speed_rpm;
        if (motor_state.direction == MOTOR_DIR_REVERSE) {
            target = -target;
        }
        Motor_UpdatePIDTarget(target);
    }
}

// ============================= Speed Gear 模式接口 =============================

/**
 * @brief 设置速度档位
 * @param gear 目标档位
 */
void MotorApp_SpeedGear_SetGear(SpeedGear gear)
{
    if (gear > SPEED_GEAR_HIGH) return;  // 超出范围

    motor_state.current_gear = gear;
    motor_state.target_rpm = gear_speeds[gear];

    // 如果正在运行,立即更新PWM
    if (motor_state.is_running && motor_state.mode == MOTOR_MODE_SPEED_GEAR) {
        Motor_SetPWM(Motor_GetCurrentModePWM());
    }
}

/**
 * @brief 增加档位(Low→Mid→High)
 */
void MotorApp_SpeedGear_IncreaseGear(void)
{
    if (motor_state.current_gear < SPEED_GEAR_HIGH) {
        MotorApp_SpeedGear_SetGear(motor_state.current_gear + 1);
    }
}

/**
 * @brief 减少档位(High→Mid→Low)
 */
void MotorApp_SpeedGear_DecreaseGear(void)
{
    if (motor_state.current_gear > SPEED_GEAR_LOW) {
        MotorApp_SpeedGear_SetGear(motor_state.current_gear - 1);
    }
}

// ============================= Acceleration 模式接口 =============================

/**
 * @brief 设置加速度模式
 * @param mode 加速度模式(Low/High)
 */
void MotorApp_Acceleration_SetMode(AccelMode mode)
{
    motor_state.accel_mode = mode;
}

/**
 * @brief 切换加速度模式(Low ↔ High)
 */
void MotorApp_Acceleration_ToggleMode(void)
{
    motor_state.accel_mode = (motor_state.accel_mode == ACCEL_MODE_LOW) ?
                              ACCEL_MODE_HIGH : ACCEL_MODE_LOW;
}

// ============================= Circle Control 模式接口 =============================

/**
 * @brief 设置目标圈数
 * @param circles 目标圈数(1-20)
 */
void MotorApp_CircleControl_SetTarget(uint8_t circles)
{
    // 限制范围 1-20
    if (circles < 1) circles = 1;
    if (circles > 20) circles = 20;

    motor_state.target_circles = circles;
}

/**
 * @brief 增加目标圈数(+1圈,最大20)
 */
void MotorApp_CircleControl_IncreaseTarget(void)
{
    if (motor_state.target_circles < 20) {
        motor_state.target_circles++;
    }
}

/**
 * @brief 减少目标圈数(-1圈,最小1)
 */
void MotorApp_CircleControl_DecreaseTarget(void)
{
    if (motor_state.target_circles > 1) {
        motor_state.target_circles--;
    }
}

// ============================= 状态查询接口 =============================

/**
 * @brief 获取电机状态指针
 * @return 电机状态结构体指针
 */
MotorState* MotorApp_GetState(void)
{
    return &motor_state;
}

/**
 * @brief 获取当前平均转速
 * @return 当前转速(rpm)
 */
float MotorApp_GetCurrentRPM(void)
{
    return motor_state.current_rpm;
}

/**
 * @brief 查询电机是否在运行
 * @return 1=运行中, 0=停止
 */
uint8_t MotorApp_IsRunning(void)
{
    return motor_state.is_running;
}
