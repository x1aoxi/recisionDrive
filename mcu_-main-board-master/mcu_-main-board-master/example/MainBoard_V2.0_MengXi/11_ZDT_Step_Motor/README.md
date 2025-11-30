# 11_ZDT_Step_Motor - 闭环步进电机控制示例

[![HAL](https://img.shields.io/badge/HAL-STM32F4-brightgreen.svg)](/Drivers) [![Keil](https://img.shields.io/badge/IDE-Keil_MDK-blue.svg)](https://www.keil.com/) 

## 项目简介

11_ZDT_Step_Motor是STM32F407VET6开发板的闭环步进电机控制示例工程，演示了如何使用编码器反馈实现高精度的步进电机闭环控制。该项目提供了完整的闭环步进电机控制方案，包括位置控制、速度控制、加减速控制、失步检测、堵转保护等功能，相比开环控制具有更高的精度和可靠性。

## 主要特性

- **闭环控制**: 基于编码器反馈的位置和速度闭环控制
- **高精度定位**: 微步驱动结合编码器反馈，实现高精度定位
- **失步检测**: 实时检测和补偿步进电机失步现象
- **平滑运行**: S曲线加减速，运行平稳无冲击
- **多种模式**: 支持位置模式、速度模式、回原点等
- **保护功能**: 堵转检测、过流保护、限位保护
- **实时监控**: 实时显示位置、速度、电流等参数
- **参数调节**: 支持运行参数在线调节和优化

## 硬件连接

### 步进电机驱动器连接

| 信号 | STM32引脚 | 功能描述 |
|------|-----------|----------|
| PUL+ | PA8 (TIM1_CH1) | 脉冲信号(+) |
| PUL- | GND | 脉冲信号(-) |
| DIR+ | PA9 | 方向信号(+) |
| DIR- | GND | 方向信号(-) |
| ENA+ | PA10 | 使能信号(+) |
| ENA- | GND | 使能信号(-) |

### 编码器反馈连接

| 编码器引脚 | STM32引脚 | 功能描述 |
|-----------|-----------|----------|
| A+ | PA0 (TIM2_CH1) | A相信号(+) |
| A- | GND | A相信号(-) |
| B+ | PA1 (TIM2_CH2) | B相信号(+) |
| B- | GND | B相信号(-) |
| Z+ | PA2 | Z相信号(零位) |
| Z- | GND | Z相信号(-) |

### 限位开关连接

| 开关 | STM32引脚 | 功能描述 |
|------|-----------|----------|
| 正限位 | PB0 | 正方向限位开关 |
| 负限位 | PB1 | 负方向限位开关 |
| 原点 | PB2 | 原点传感器(可选) |

### 典型连接图

```
     步进驱动器                  STM32F407VET6
    ┌─────────────┐              ┌──────────────┐
    │ PUL+        │←────────────→│ PA8(TIM1_CH1)│
    │ PUL-        │←────────────→│ GND          │
    │ DIR+        │←────────────→│ PA9          │
    │ DIR-        │←────────────→│ GND          │
    │ ENA+        │←────────────→│ PA10         │
    │ ENA-        │←────────────→│ GND          │
    └─────────────┘              └──────────────┘
            │
    ┌─────────────┐              ┌──────────────┐
    │  步进电机    │              │   编码器     │
    │             │              │ A+ ─────────→│ PA0(TIM2_CH1)│
    │             │              │ B+ ─────────→│ PA1(TIM2_CH2)│
    │             │              │ Z+ ─────────→│ PA2          │
    │             │              │ GND ────────→│ GND          │
    └─────────────┘              └──────────────┘
```

## 目录结构

```
11_ZDT_Step_Motor/
├── Core/
│   ├── Inc/                     # 核心头文件
│   │   ├── main.h
│   │   ├── gpio.h
│   │   ├── usart.h
│   │   ├── tim.h
│   │   └── it.h
│   └── Src/                     # 核心源文件
│       ├── main.c
│       ├── gpio.c
│       ├── usart.c
│       ├── tim.c
│       └── it.c
├── Drivers/
│   ├── CMSIS/                   # CMSIS库
│   └── STM32F4xx_HAL_Driver/    # HAL驱动库
├── MDK-ARM/
│   ├── 11_ZDT_Motor.uvprojx     # Keil工程文件
│   └── startup_stm32f407xx.s    # 启动文件
├── User/
│   ├── MyDefine.h               # 用户定义
│   ├── Scheduler.c              # 任务调度器
│   ├── Scheduler.h
│   ├── Scheduler_Task.c         # 任务实现
│   ├── Scheduler_Task.h
│   ├── App/
│   │   ├── step_motor_app.c     # 步进电机应用
│   │   ├── step_motor_app.h
│   │   ├── motion_planner.c     # 运动规划器
│   │   ├── motion_planner.h
│   │   ├── homing_control.c     # 回原点控制
│   │   └── homing_control.h
│   ├── Driver/
│   │   ├── step_motor_driver.c  # 步进电机驱动
│   │   ├── step_motor_driver.h
│   │   ├── encoder_feedback.c   # 编码器反馈
│   │   ├── encoder_feedback.h
│   │   ├── limit_switch.c       # 限位开关
│   │   └── limit_switch.h
│   └── Module/
│       ├── closed_loop_ctrl.c   # 闭环控制模块
│       ├── closed_loop_ctrl.h
│       ├── trajectory_gen.c     # 轨迹生成
│       ├── trajectory_gen.h
│       ├── error_detection.c    # 误差检测
│       └── error_detection.h
├── 11_ZDT_Motor.ioc            # CubeMX配置文件
└── keilkilll.bat               # Keil进程清理脚本
```

## 使用方法

### 快速开始

1. **硬件连接**
   - 按照连接表连接步进驱动器
   - 连接编码器到相应引脚
   - 安装限位开关(可选)

2. **编译下载**
   ```bash
   # 打开Keil工程
   双击 MDK-ARM/11_ZDT_Motor.uvprojx
   # 编译工程 (F7)
   # 下载程序 (F8)
   ```

3. **运行测试**
   - 打开串口终端(115200,8,N,1)
   - 执行回原点操作
   - 发送运动命令测试

### 代码示例

#### 闭环步进电机驱动

```c
#include "step_motor_driver.h"
#include "encoder_feedback.h"

/* 步进电机状态枚举 */
typedef enum {
    MOTOR_IDLE = 0,
    MOTOR_RUNNING,
    MOTOR_ACCELERATING,
    MOTOR_DECELERATING,
    MOTOR_STOPPING,
    MOTOR_ERROR
} motor_state_t;

/* 运动模式枚举 */
typedef enum {
    MODE_POSITION = 0,          // 位置模式
    MODE_VELOCITY,              // 速度模式
    MODE_HOMING,                // 回原点模式
    MODE_JOG                    // 点动模式
} motion_mode_t;

/* 步进电机控制结构体 */
typedef struct {
    /* 基本参数 */
    int32_t target_position;    // 目标位置(脉冲)
    int32_t current_position;   // 当前位置(脉冲)
    float target_velocity;      // 目标速度(pps)
    float current_velocity;     // 当前速度(pps)
    
    /* 运动参数 */
    float max_velocity;         // 最大速度(pps)
    float acceleration;         // 加速度(pps²)
    float deceleration;         // 减速度(pps²)
    uint16_t microstep;         // 微步设置
    
    /* 控制状态 */
    motor_state_t state;        // 电机状态
    motion_mode_t mode;         // 运动模式
    uint8_t enable;             // 使能状态
    uint8_t direction;          // 运动方向
    
    /* 闭环控制 */
    int32_t encoder_position;   // 编码器位置
    int32_t position_error;     // 位置误差
    float position_kp;          // 位置比例增益
    uint8_t closed_loop_enable; // 闭环使能
    
    /* 保护参数 */
    int32_t positive_limit;     // 正向软限位
    int32_t negative_limit;     // 负向软限位
    uint8_t limit_switch_state; // 限位开关状态
    uint32_t stall_time;        // 堵转时间
    
    /* 统计信息 */
    uint32_t step_count;        // 步数计数
    uint32_t run_time;          // 运行时间
    uint16_t error_count;       // 错误计数
} step_motor_t;

step_motor_t step_motor;

/* 步进电机初始化 */
void Step_Motor_Init(void)
{
    /* 初始化GPIO */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);   // DIR = 0
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);  // ENA = 0 (禁用)
    
    /* 初始化PWM */
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);  // 初始占空比为0
    
    /* 初始化编码器 */
    Encoder_Feedback_Init();
    
    /* 初始化参数 */
    step_motor.target_position = 0;
    step_motor.current_position = 0;
    step_motor.target_velocity = 0;
    step_motor.current_velocity = 0;
    
    step_motor.max_velocity = 2000.0f;      // 最大2000pps
    step_motor.acceleration = 1000.0f;      // 1000pps²
    step_motor.deceleration = 1000.0f;
    step_motor.microstep = 16;              // 16微步
    
    step_motor.state = MOTOR_IDLE;
    step_motor.mode = MODE_POSITION;
    step_motor.enable = 0;
    step_motor.direction = 0;
    
    step_motor.encoder_position = 0;
    step_motor.position_error = 0;
    step_motor.position_kp = 2.0f;          // 位置环增益
    step_motor.closed_loop_enable = 1;      // 默认使能闭环
    
    step_motor.positive_limit = 100000;     // 软限位
    step_motor.negative_limit = -100000;
    step_motor.limit_switch_state = 0;
    step_motor.stall_time = 0;
    
    step_motor.step_count = 0;
    step_motor.run_time = 0;
    step_motor.error_count = 0;
    
    printf("Step Motor Initialized\\r\\n");
    printf("Max Speed: %.0f pps, Accel: %.0f pps²\\r\\n", 
           step_motor.max_velocity, step_motor.acceleration);
}

/* 步进电机使能控制 */
void Step_Motor_Enable(uint8_t enable)
{
    step_motor.enable = enable;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);  // 低电平使能
    
    if(enable)
    {
        printf("Step Motor Enabled\\r\\n");
    }
    else
    {
        Step_Motor_Stop();
        printf("Step Motor Disabled\\r\\n");
    }
}

/* 设置目标位置 */
void Step_Motor_Set_Position(int32_t position)
{
    if(!step_motor.enable) return;
    
    /* 检查软限位 */
    if(position > step_motor.positive_limit)
        position = step_motor.positive_limit;
    else if(position < step_motor.negative_limit)
        position = step_motor.negative_limit;
    
    step_motor.target_position = position;
    step_motor.mode = MODE_POSITION;
    step_motor.state = MOTOR_RUNNING;
    
    printf("Position Target Set: %ld\\r\\n", position);
}

/* 设置目标速度 */
void Step_Motor_Set_Velocity(float velocity)
{
    if(!step_motor.enable) return;
    
    /* 速度限制 */
    if(velocity > step_motor.max_velocity)
        velocity = step_motor.max_velocity;
    else if(velocity < -step_motor.max_velocity)
        velocity = -step_motor.max_velocity;
    
    step_motor.target_velocity = velocity;
    step_motor.mode = MODE_VELOCITY;
    step_motor.state = MOTOR_RUNNING;
    
    printf("Velocity Target Set: %.1f pps\\r\\n", velocity);
}

/* 步进电机停止 */
void Step_Motor_Stop(void)
{
    step_motor.target_velocity = 0;
    step_motor.state = MOTOR_STOPPING;
    
    printf("Step Motor Stopping...\\r\\n");
}

/* 急停 */
void Step_Motor_Emergency_Stop(void)
{
    step_motor.target_velocity = 0;
    step_motor.current_velocity = 0;
    step_motor.state = MOTOR_IDLE;
    
    /* 立即停止PWM输出 */
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
    
    printf("Emergency Stop!\\r\\n");
}

/* 步进电机控制更新 */
void Step_Motor_Control_Update(void)
{
    static uint32_t last_update = 0;
    uint32_t current_time = HAL_GetTick();
    
    if(current_time - last_update < 10) return;  // 10ms更新周期
    
    /* 更新编码器反馈 */
    step_motor.encoder_position = Encoder_Get_Position();
    
    /* 闭环位置控制 */
    if(step_motor.closed_loop_enable)
    {
        Step_Motor_Closed_Loop_Control();
    }
    
    /* 轨迹规划和速度控制 */
    Step_Motor_Trajectory_Planning();
    
    /* 更新PWM输出 */
    Step_Motor_Update_PWM();
    
    /* 错误检测 */
    Step_Motor_Error_Detection();
    
    last_update = current_time;
}

/* 闭环位置控制 */
void Step_Motor_Closed_Loop_Control(void)
{
    /* 计算位置误差 */
    step_motor.position_error = step_motor.current_position - step_motor.encoder_position;
    
    /* 位置误差补偿 */
    if(step_motor.mode == MODE_POSITION)
    {
        float error_correction = step_motor.position_error * step_motor.position_kp;
        
        /* 限制补偿量 */
        if(error_correction > 100.0f)
            error_correction = 100.0f;
        else if(error_correction < -100.0f)
            error_correction = -100.0f;
        
        /* 应用到目标速度 */
        step_motor.target_velocity += error_correction;
    }
}

/* 轨迹规划 */
void Step_Motor_Trajectory_Planning(void)
{
    float dt = 0.01f;  // 10ms时间步长
    
    switch(step_motor.mode)
    {
        case MODE_POSITION:
            /* 位置模式轨迹规划 */
            {
                int32_t position_error = step_motor.target_position - step_motor.current_position;
                float target_vel;
                
                if(abs(position_error) < 10)  // 到达目标位置
                {
                    target_vel = 0;
                    if(fabs(step_motor.current_velocity) < 1.0f)
                    {
                        step_motor.state = MOTOR_IDLE;
                    }
                }
                else
                {
                    /* 梯形速度规划 */
                    float max_vel_for_distance = sqrt(2 * step_motor.acceleration * abs(position_error));
                    target_vel = (max_vel_for_distance < step_motor.max_velocity) ? 
                                max_vel_for_distance : step_motor.max_velocity;
                    
                    if(position_error < 0)
                        target_vel = -target_vel;
                }
                
                step_motor.target_velocity = target_vel;
            }
            break;
            
        case MODE_VELOCITY:
            /* 速度模式：目标速度由外部设定 */
            break;
            
        case MODE_HOMING:
            /* 回原点模式 */
            Step_Motor_Homing_Update();
            break;
    }
    
    /* 速度斜坡控制 */
    float velocity_error = step_motor.target_velocity - step_motor.current_velocity;
    float max_vel_change = step_motor.acceleration * dt;
    
    if(velocity_error > max_vel_change)
    {
        step_motor.current_velocity += max_vel_change;
        step_motor.state = MOTOR_ACCELERATING;
    }
    else if(velocity_error < -max_vel_change)
    {
        step_motor.current_velocity -= max_vel_change;
        step_motor.state = MOTOR_DECELERATING;
    }
    else
    {
        step_motor.current_velocity = step_motor.target_velocity;
        if(fabs(step_motor.current_velocity) < 1.0f)
        {
            step_motor.state = MOTOR_IDLE;
        }
        else
        {
            step_motor.state = MOTOR_RUNNING;
        }
    }
}

/* 更新PWM输出 */
void Step_Motor_Update_PWM(void)
{
    if(fabs(step_motor.current_velocity) < 1.0f)
    {
        /* 停止状态 */
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        return;
    }
    
    /* 设置方向 */
    uint8_t direction = (step_motor.current_velocity > 0) ? 1 : 0;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, direction ? GPIO_PIN_SET : GPIO_PIN_RESET);
    step_motor.direction = direction;
    
    /* 计算PWM频率 */
    float abs_velocity = fabs(step_motor.current_velocity);
    uint32_t timer_freq = HAL_RCC_GetPCLK2Freq() * 2;  // TIM1时钟频率
    uint32_t period = timer_freq / (abs_velocity * 2);  // 50%占空比
    
    /* 限制频率范围 */
    if(period < 100) period = 100;      // 最高频率限制
    if(period > 100000) period = 100000; // 最低频率限制
    
    /* 更新定时器周期 */
    __HAL_TIM_SET_AUTORELOAD(&htim1, period - 1);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, period / 2);  // 50%占空比
    
    /* 更新位置 */
    static uint32_t last_step_time = 0;
    uint32_t current_time = HAL_GetTick();
    if(current_time != last_step_time)
    {
        float steps_per_ms = abs_velocity / 1000.0f;
        int32_t step_increment = (int32_t)(steps_per_ms * (current_time - last_step_time));
        
        if(direction)
            step_motor.current_position += step_increment;
        else
            step_motor.current_position -= step_increment;
        
        step_motor.step_count += step_increment;
        last_step_time = current_time;
    }
}

/* 错误检测 */
void Step_Motor_Error_Detection(void)
{
    /* 检查限位开关 */
    uint8_t pos_limit = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
    uint8_t neg_limit = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
    
    if(pos_limit && step_motor.direction == 1)  // 正向限位
    {
        Step_Motor_Emergency_Stop();
        step_motor.error_count++;
        printf("Positive Limit Hit!\\r\\n");
    }
    
    if(neg_limit && step_motor.direction == 0)  // 负向限位
    {
        Step_Motor_Emergency_Stop();
        step_motor.error_count++;
        printf("Negative Limit Hit!\\r\\n");
    }
    
    /* 检查位置误差 */
    if(step_motor.closed_loop_enable && abs(step_motor.position_error) > 1000)
    {
        static uint32_t error_start_time = 0;
        if(error_start_time == 0)
            error_start_time = HAL_GetTick();
        else if(HAL_GetTick() - error_start_time > 2000)  // 2秒持续误差
        {
            Step_Motor_Emergency_Stop();
            step_motor.error_count++;
            printf("Position Error Too Large: %ld\\r\\n", step_motor.position_error);
            error_start_time = 0;
        }
    }
    else
    {
        /* 重置错误计时 */
        static uint32_t* error_start_time = NULL;
        if(error_start_time) *error_start_time = 0;
    }
}

/* 回原点控制 */
void Step_Motor_Homing_Update(void)
{
    static uint8_t homing_phase = 0;  // 0: 寻找负限位, 1: 离开限位, 2: 寻找Z相, 3: 完成
    
    uint8_t neg_limit = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
    uint8_t z_signal = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2);
    
    switch(homing_phase)
    {
        case 0:  // 寻找负限位
            step_motor.target_velocity = -500.0f;  // 慢速负向运动
            if(neg_limit)
            {
                homing_phase = 1;
                printf("Negative limit found\\r\\n");
            }
            break;
            
        case 1:  // 离开限位
            step_motor.target_velocity = 100.0f;   // 慢速正向离开
            if(!neg_limit)
            {
                homing_phase = 2;
                printf("Left negative limit\\r\\n");
            }
            break;
            
        case 2:  // 寻找Z相信号
            step_motor.target_velocity = 50.0f;    // 极慢速寻找Z相
            if(z_signal)
            {
                homing_phase = 3;
                step_motor.current_position = 0;    // 设置原点
                step_motor.encoder_position = 0;
                Encoder_Reset_Position();
                printf("Z-phase found, homing complete\\r\\n");
            }
            break;
            
        case 3:  // 回原点完成
            step_motor.target_velocity = 0;
            step_motor.mode = MODE_POSITION;
            step_motor.state = MOTOR_IDLE;
            homing_phase = 0;  // 复位回原点状态
            break;
    }
}

/* 获取步进电机状态 */
step_motor_t* Step_Motor_Get_Status(void)
{
    return &step_motor;
}
```

#### 编码器反馈处理

```c
#include "encoder_feedback.h"

/* 编码器数据结构 */
typedef struct {
    int32_t position;           // 绝对位置
    int32_t raw_count;          // 原始计数
    float velocity;             // 速度
    uint16_t resolution;        // 编码器分辨率
    float gear_ratio;           // 减速比
    uint32_t update_time;       // 更新时间
} encoder_feedback_t;

encoder_feedback_t encoder;

/* 编码器初始化 */
void Encoder_Feedback_Init(void)
{
    /* 启动编码器模式 */
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    
    /* 设置定时器中点值 */
    __HAL_TIM_SET_COUNTER(&htim2, 32768);
    
    /* 初始化参数 */
    encoder.position = 0;
    encoder.raw_count = 0;
    encoder.velocity = 0;
    encoder.resolution = 2500;  // 2500线编码器
    encoder.gear_ratio = 1.0f;  // 直连，无减速
    encoder.update_time = HAL_GetTick();
    
    printf("Encoder Feedback Initialized (Resolution: %d)\\r\\n", encoder.resolution);
}

/* 获取编码器位置 */
int32_t Encoder_Get_Position(void)
{
    /* 读取定时器计数 */
    uint16_t tim_count = __HAL_TIM_GET_COUNTER(&htim2);
    encoder.raw_count = (int32_t)tim_count - 32768;
    
    /* 转换为绝对位置 */
    encoder.position = encoder.raw_count;
    
    return encoder.position;
}

/* 计算编码器速度 */
float Encoder_Get_Velocity(void)
{
    static int32_t last_position = 0;
    static uint32_t last_time = 0;
    
    uint32_t current_time = HAL_GetTick();
    int32_t current_position = Encoder_Get_Position();
    
    if(current_time != last_time)
    {
        int32_t delta_position = current_position - last_position;
        uint32_t delta_time = current_time - last_time;
        
        /* 计算速度 (位置/秒) */
        encoder.velocity = (float)delta_position * 1000.0f / delta_time;
        
        last_position = current_position;
        last_time = current_time;
    }
    
    return encoder.velocity;
}

/* 复位编码器位置 */
void Encoder_Reset_Position(void)
{
    __HAL_TIM_SET_COUNTER(&htim2, 32768);
    encoder.position = 0;
    encoder.raw_count = 0;
    
    printf("Encoder Position Reset\\r\\n");
}

/* 获取编码器数据 */
encoder_feedback_t* Encoder_Get_Data(void)
{
    Encoder_Get_Position();
    Encoder_Get_Velocity();
    encoder.update_time = HAL_GetTick();
    
    return &encoder;
}
```

#### 串口命令处理

```c
/* 步进电机命令处理 */
void Step_Motor_Process_Command(char* cmd)
{
    step_motor_t* motor = Step_Motor_Get_Status();
    
    if(strncmp(cmd, "ENABLE", 6) == 0)
    {
        Step_Motor_Enable(1);
    }
    else if(strncmp(cmd, "DISABLE", 7) == 0)
    {
        Step_Motor_Enable(0);
    }
    else if(strncmp(cmd, "POS", 3) == 0)
    {
        int32_t position;
        if(sscanf(cmd, "POS %ld", &position) == 1)
        {
            Step_Motor_Set_Position(position);
        }
        else
        {
            printf("Current Position: %ld\\r\\n", motor->current_position);
        }
    }
    else if(strncmp(cmd, "VEL", 3) == 0)
    {
        float velocity;
        if(sscanf(cmd, "VEL %f", &velocity) == 1)
        {
            Step_Motor_Set_Velocity(velocity);
        }
        else
        {
            printf("Current Velocity: %.1f pps\\r\\n", motor->current_velocity);
        }
    }
    else if(strncmp(cmd, "HOME", 4) == 0)
    {
        motor->mode = MODE_HOMING;
        motor->state = MOTOR_RUNNING;
        printf("Homing Started\\r\\n");
    }
    else if(strncmp(cmd, "STOP", 4) == 0)
    {
        Step_Motor_Stop();
    }
    else if(strncmp(cmd, "ESTOP", 5) == 0)
    {
        Step_Motor_Emergency_Stop();
    }
    else if(strncmp(cmd, "STATUS", 6) == 0)
    {
        encoder_feedback_t* encoder = Encoder_Get_Data();
        
        printf("\\r\\n=== Step Motor Status ===\\r\\n");
        printf("Enable: %s\\r\\n", motor->enable ? "ON" : "OFF");
        printf("State: %d, Mode: %d\\r\\n", motor->state, motor->mode);
        printf("Position: %ld (Target: %ld)\\r\\n", 
               motor->current_position, motor->target_position);
        printf("Velocity: %.1f pps (Target: %.1f)\\r\\n", 
               motor->current_velocity, motor->target_velocity);
        printf("Encoder: %ld (Error: %ld)\\r\\n", 
               encoder->position, motor->position_error);
        printf("Steps: %ld, Errors: %d\\r\\n", 
               motor->step_count, motor->error_count);
        printf("========================\\r\\n");
    }
    else if(strncmp(cmd, "PARAM", 5) == 0)
    {
        float max_vel, accel, kp;
        if(sscanf(cmd, "PARAM %f %f %f", &max_vel, &accel, &kp) == 3)
        {
            motor->max_velocity = max_vel;
            motor->acceleration = accel;
            motor->deceleration = accel;
            motor->position_kp = kp;
            printf("Parameters Updated: MaxVel=%.0f, Accel=%.0f, Kp=%.2f\\r\\n",
                   max_vel, accel, kp);
        }
        else
        {
            printf("Current Params: MaxVel=%.0f, Accel=%.0f, Kp=%.2f\\r\\n",
                   motor->max_velocity, motor->acceleration, motor->position_kp);
        }
    }
    else
    {
        printf("Step Motor Commands:\\r\\n");
        printf("  ENABLE/DISABLE - Enable/disable motor\\r\\n");
        printf("  POS <value> - Set target position\\r\\n");
        printf("  VEL <value> - Set target velocity\\r\\n");
        printf("  HOME - Start homing sequence\\r\\n");
        printf("  STOP/ESTOP - Stop/emergency stop\\r\\n");
        printf("  STATUS - Show motor status\\r\\n");
        printf("  PARAM [maxvel accel kp] - Set/show parameters\\r\\n");
    }
}
```

## API参考

### 步进电机控制函数

```c
/* 基础控制 */
void Step_Motor_Init(void);                            // 步进电机初始化
void Step_Motor_Enable(uint8_t enable);                // 使能控制
void Step_Motor_Set_Position(int32_t position);        // 设置目标位置
void Step_Motor_Set_Velocity(float velocity);          // 设置目标速度
void Step_Motor_Stop(void);                            // 停止
void Step_Motor_Emergency_Stop(void);                  // 急停
step_motor_t* Step_Motor_Get_Status(void);             // 获取状态
```

### 控制更新函数

```c
/* 控制更新 */
void Step_Motor_Control_Update(void);                  // 控制更新
void Step_Motor_Closed_Loop_Control(void);             // 闭环控制
void Step_Motor_Trajectory_Planning(void);             // 轨迹规划
void Step_Motor_Update_PWM(void);                      // 更新PWM
void Step_Motor_Error_Detection(void);                 // 错误检测
void Step_Motor_Homing_Update(void);                   // 回原点更新
```

### 编码器反馈函数

```c
/* 编码器反馈 */
void Encoder_Feedback_Init(void);                      // 编码器初始化
int32_t Encoder_Get_Position(void);                    // 获取位置
float Encoder_Get_Velocity(void);                      // 获取速度
void Encoder_Reset_Position(void);                     // 复位位置
encoder_feedback_t* Encoder_Get_Data(void);            // 获取数据
```

## 配置选项

### 电机参数配置

```c
/* 在step_motor_driver.h中配置 */
#define DEFAULT_MAX_VELOCITY    2000.0f     // 默认最大速度(pps)
#define DEFAULT_ACCELERATION    1000.0f     // 默认加速度(pps²)
#define DEFAULT_MICROSTEP      16           // 默认微步数
#define POSITION_KP            2.0f         // 位置环比例增益
```

### 编码器配置

```c
/* 编码器配置 */
#define ENCODER_RESOLUTION     2500         // 编码器分辨率
#define GEAR_RATIO             1.0f         // 减速比
#define ENCODER_TIMER          htim2        // 编码器定时器
```

### 保护参数配置

```c
/* 保护配置 */
#define SOFT_LIMIT_POSITIVE    100000       // 正向软限位
#define SOFT_LIMIT_NEGATIVE    -100000      // 负向软限位
#define MAX_POSITION_ERROR     1000         // 最大位置误差
#define ERROR_DETECTION_TIME   2000         // 错误检测时间(ms)
```

## 使用场景

1. **精密定位**: 需要高精度定位的自动化设备
2. **数控机床**: CNC机床的进给轴控制
3. **3D打印机**: 各轴精密控制系统
4. **机器人关节**: 机器人关节的精确控制
5. **包装设备**: 包装生产线的定位控制

## 故障排除

### 常见问题

1. **失步现象**
   - 检查负载是否过重
   - 调整加速度参数
   - 验证驱动器设置

2. **位置误差大**
   - 调整闭环增益参数
   - 检查编码器安装
   - 验证机械间隙

3. **振动噪音**
   - 调整微步设置
   - 优化速度曲线
   - 检查机械共振

## 学习要点

1. **闭环控制**: 学习闭环步进电机的优势和原理
2. **轨迹规划**: 理解运动轨迹规划算法
3. **编码器应用**: 掌握编码器反馈在精密控制中的作用
4. **错误检测**: 了解步进电机的各种故障模式
5. **参数调优**: 学习控制参数的调试方法

## 版权信息

版权 (c) 2025 MCU Electronics Technology

MIT License - 详见LICENSE文件
