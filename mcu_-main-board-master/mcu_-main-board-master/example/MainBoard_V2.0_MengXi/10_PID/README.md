# 10_PID - PID控制算法示例

[![HAL](https://img.shields.io/badge/HAL-STM32F4-brightgreen.svg)](/Drivers) [![Keil](https://img.shields.io/badge/IDE-Keil_MDK-blue.svg)](https://www.keil.com/) 

## 项目简介

10_PID是STM32F407VET6开发板的PID控制算法示例工程，演示了如何实现和应用经典的PID(比例-积分-微分)控制算法。该项目提供了完整的PID控制方案，包括位置式PID、增量式PID、抗积分饱和、参数自整定等功能，适用于温度控制、电机调速、位置控制等各种反馈控制应用。

## 主要特性

- **双模式PID**: 支持位置式和增量式PID算法
- **参数整定**: 多种PID参数整定方法和工具
- **抗饱和处理**: 积分抗饱和和输出限幅功能
- **滤波器**: 微分项低通滤波，减少高频噪声
- **自适应控制**: 支持参数自适应调整
- **实时监控**: 实时显示控制过程和参数
- **多种应用**: 温度控制、电机调速、位置控制等实例
- **参数保存**: 最优参数的存储和恢复功能

## 硬件连接

### 温度控制系统连接

| 组件 | STM32引脚 | 功能描述 |
|------|-----------|----------|
| 温度传感器 | PA4 (ADC1_IN4) | 温度反馈(0-3.3V) |
| 加热器PWM | PA8 (TIM1_CH1) | 加热器控制输出 |
| LED指示 | PC0-PC3 | 控制状态指示 |
| 按键 | PA0-PA3 | 参数调节按键 |

### 电机调速系统连接

| 组件 | STM32引脚 | 功能描述 |
|------|-----------|----------|
| 编码器A相 | PA0 (TIM2_CH1) | 速度反馈 |
| 编码器B相 | PA1 (TIM2_CH2) | 方向检测 |
| 电机PWM | PA8 (TIM1_CH1) | 电机调速输出 |
| 方向控制 | PC6, PC7 | 电机方向控制 |

### 位置控制系统连接

| 组件 | STM32引脚 | 功能描述 |
|------|-----------|----------|
| 位置传感器 | PA5 (ADC1_IN5) | 位置反馈(电位器) |
| 伺服PWM | PA9 (TIM1_CH2) | 伺服电机控制 |
| 限位开关 | PB0, PB1 | 行程限位检测 |

## 目录结构

```
10_PID/
├── Core/
│   ├── Inc/                     # 核心头文件
│   │   ├── main.h
│   │   ├── gpio.h
│   │   ├── usart.h
│   │   ├── tim.h
│   │   ├── adc.h
│   │   └── dma.h
│   └── Src/                     # 核心源文件
│       ├── main.c
│       ├── gpio.c
│       ├── usart.c
│       ├── tim.c
│       ├── adc.c
│       └── dma.c
├── Drivers/
│   ├── CMSIS/                   # CMSIS库
│   └── STM32F4xx_HAL_Driver/    # HAL驱动库
├── MDK-ARM/
│   ├── 10_PID.uvprojx           # Keil工程文件
│   └── startup_stm32f407xx.s    # 启动文件
├── User/
│   ├── MyDefine.h               # 用户定义
│   ├── Scheduler.c              # 任务调度器
│   ├── Scheduler.h
│   ├── Scheduler_Task.c         # 任务实现
│   ├── Scheduler_Task.h
│   ├── App/
│   │   ├── temperature_control.c # 温度控制应用
│   │   ├── temperature_control.h
│   │   ├── speed_control.c      # 速度控制应用
│   │   ├── speed_control.h
│   │   ├── position_control.c   # 位置控制应用
│   │   ├── position_control.h
│   │   ├── pid_tuner.c          # PID整定工具
│   │   └── pid_tuner.h
│   ├── Driver/
│   │   ├── pid_controller.c     # PID控制器驱动
│   │   ├── pid_controller.h
│   │   ├── sensor_driver.c      # 传感器驱动
│   │   └── sensor_driver.h
│   └── Module/
│       ├── pid_algorithms.c     # PID算法模块
│       ├── pid_algorithms.h
│       ├── auto_tune.c          # 自动整定模块
│       ├── auto_tune.h
│       ├── data_logger.c        # 数据记录模块
│       └── data_logger.h
├── 10_PID.ioc                  # CubeMX配置文件
└── keilkilll.bat               # Keil进程清理脚本
```

## 使用方法

### 快速开始

1. **硬件连接**
   - 根据应用场景连接相应的传感器和执行器
   - 确保反馈信号连接正确
   - 检查电源和地线连接

2. **编译下载**
   ```bash
   # 打开Keil工程
   双击 MDK-ARM/10_PID.uvprojx
   # 编译工程 (F7)
   # 下载程序 (F8)
   ```

3. **运行测试**
   - 打开串口终端(115200,8,N,1)
   - 选择控制模式和设定值
   - 观察控制效果和参数调节

### 代码示例

#### PID算法核心实现

```c
#include "pid_algorithms.h"

/* PID控制器结构体 */
typedef struct {
    /* PID参数 */
    float Kp;                   // 比例系数
    float Ki;                   // 积分系数
    float Kd;                   // 微分系数
    
    /* 控制变量 */
    float setpoint;             // 设定值
    float input;                // 当前输入(反馈值)
    float output;               // 控制输出
    float error;                // 当前误差
    
    /* 历史数据 */
    float last_error;           // 上次误差
    float integral;             // 积分累积
    float derivative;           // 微分项
    
    /* 限制参数 */
    float output_min;           // 输出最小值
    float output_max;           // 输出最大值
    float integral_min;         // 积分最小值
    float integral_max;         // 积分最大值
    
    /* 控制选项 */
    uint8_t mode;               // 控制模式: 0=手动, 1=自动
    uint8_t direction;          // 控制方向: 0=正向, 1=反向
    uint32_t sample_time;       // 采样时间(ms)
    uint32_t last_time;         // 上次计算时间
    
    /* 滤波参数 */
    float d_filter_alpha;       // 微分项低通滤波系数
    float last_derivative;      // 上次微分值
    
    /* 抗饱和参数 */
    uint8_t anti_windup;        // 抗饱和使能
    float windup_limit;         // 饱和限制
    
} pid_controller_t;

/* PID控制器初始化 */
void PID_Init(pid_controller_t* pid, float kp, float ki, float kd)
{
    /* 设置PID参数 */
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    
    /* 初始化变量 */
    pid->setpoint = 0.0f;
    pid->input = 0.0f;
    pid->output = 0.0f;
    pid->error = 0.0f;
    
    /* 清除历史数据 */
    pid->last_error = 0.0f;
    pid->integral = 0.0f;
    pid->derivative = 0.0f;
    pid->last_derivative = 0.0f;
    
    /* 设置默认限制 */
    pid->output_min = -100.0f;
    pid->output_max = 100.0f;
    pid->integral_min = -50.0f;
    pid->integral_max = 50.0f;
    
    /* 设置控制参数 */
    pid->mode = 1;              // 自动模式
    pid->direction = 0;         // 正向控制
    pid->sample_time = 100;     // 100ms采样
    pid->last_time = HAL_GetTick();
    
    /* 设置滤波参数 */
    pid->d_filter_alpha = 0.1f; // 微分低通滤波
    
    /* 设置抗饱和参数 */
    pid->anti_windup = 1;       // 使能抗饱和
    pid->windup_limit = 80.0f;  // 80%饱和限制
    
    printf("PID Controller Initialized: Kp=%.2f, Ki=%.2f, Kd=%.2f\\r\\n", kp, ki, kd);
}

/* 位置式PID计算 */
float PID_Compute_Position(pid_controller_t* pid, float setpoint, float input)
{
    if(pid->mode == 0) return pid->output;  // 手动模式直接返回
    
    uint32_t now = HAL_GetTick();
    uint32_t time_change = now - pid->last_time;
    
    if(time_change < pid->sample_time) return pid->output;  // 采样时间未到
    
    /* 更新输入和设定值 */
    pid->setpoint = setpoint;
    pid->input = input;
    pid->error = setpoint - input;
    
    /* 反向控制 */
    if(pid->direction == 1)
        pid->error = -pid->error;
    
    /* 比例项 */
    float proportional = pid->Kp * pid->error;
    
    /* 积分项 */
    pid->integral += pid->error * time_change / 1000.0f;
    
    /* 积分限幅 */
    if(pid->integral > pid->integral_max)
        pid->integral = pid->integral_max;
    else if(pid->integral < pid->integral_min)
        pid->integral = pid->integral_min;
    
    float integral_term = pid->Ki * pid->integral;
    
    /* 微分项 */
    float error_change = pid->error - pid->last_error;
    float derivative_raw = error_change * 1000.0f / time_change;
    
    /* 微分项低通滤波 */
    pid->derivative = pid->d_filter_alpha * derivative_raw + 
                     (1.0f - pid->d_filter_alpha) * pid->last_derivative;
    pid->last_derivative = pid->derivative;
    
    float derivative_term = pid->Kd * pid->derivative;
    
    /* PID输出计算 */
    pid->output = proportional + integral_term + derivative_term;
    
    /* 输出限幅 */
    if(pid->output > pid->output_max)
        pid->output = pid->output_max;
    else if(pid->output < pid->output_min)
        pid->output = pid->output_min;
    
    /* 抗积分饱和处理 */
    if(pid->anti_windup)
    {
        float output_range = pid->output_max - pid->output_min;
        float windup_threshold = output_range * pid->windup_limit / 100.0f;
        
        if(fabs(pid->output) > windup_threshold)
        {
            /* 达到饱和限制，停止积分累积 */
            if((pid->error > 0 && pid->output > windup_threshold) ||
               (pid->error < 0 && pid->output < -windup_threshold))
            {
                pid->integral -= pid->error * time_change / 1000.0f;
            }
        }
    }
    
    /* 更新历史数据 */
    pid->last_error = pid->error;
    pid->last_time = now;
    
    return pid->output;
}

/* 增量式PID计算 */
float PID_Compute_Incremental(pid_controller_t* pid, float setpoint, float input)
{
    static float last_last_error = 0.0f;
    
    if(pid->mode == 0) return 0.0f;  // 手动模式返回0增量
    
    uint32_t now = HAL_GetTick();
    uint32_t time_change = now - pid->last_time;
    
    if(time_change < pid->sample_time) return 0.0f;
    
    /* 更新输入和设定值 */
    pid->setpoint = setpoint;
    pid->input = input;
    pid->error = setpoint - input;
    
    /* 反向控制 */
    if(pid->direction == 1)
        pid->error = -pid->error;
    
    /* 增量式PID计算 */
    float delta_output = pid->Kp * (pid->error - pid->last_error) +
                        pid->Ki * pid->error * time_change / 1000.0f +
                        pid->Kd * (pid->error - 2 * pid->last_error + last_last_error) * 1000.0f / time_change;
    
    /* 更新输出 */
    pid->output += delta_output;
    
    /* 输出限幅 */
    if(pid->output > pid->output_max)
        pid->output = pid->output_max;
    else if(pid->output < pid->output_min)
        pid->output = pid->output_min;
    
    /* 更新历史误差 */
    last_last_error = pid->last_error;
    pid->last_error = pid->error;
    pid->last_time = now;
    
    return delta_output;
}

/* 设置PID参数 */
void PID_Set_Tunings(pid_controller_t* pid, float kp, float ki, float kd)
{
    if(kp < 0 || ki < 0 || kd < 0) return;
    
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    
    printf("PID Parameters Updated: Kp=%.3f, Ki=%.3f, Kd=%.3f\\r\\n", kp, ki, kd);
}

/* 设置输出限制 */
void PID_Set_Output_Limits(pid_controller_t* pid, float min, float max)
{
    if(min >= max) return;
    
    pid->output_min = min;
    pid->output_max = max;
    
    /* 限制当前输出 */
    if(pid->output > max)
        pid->output = max;
    else if(pid->output < min)
        pid->output = min;
    
    /* 调整积分限制 */
    float range = max - min;
    pid->integral_min = -range / 2;
    pid->integral_max = range / 2;
}

/* 设置采样时间 */
void PID_Set_Sample_Time(pid_controller_t* pid, uint32_t new_sample_time)
{
    if(new_sample_time < 1) return;
    
    float ratio = (float)new_sample_time / pid->sample_time;
    pid->Ki *= ratio;
    pid->Kd /= ratio;
    
    pid->sample_time = new_sample_time;
}

/* 设置控制方向 */
void PID_Set_Control_Direction(pid_controller_t* pid, uint8_t direction)
{
    pid->direction = direction;
    printf("PID Control Direction: %s\\r\\n", direction ? "Reverse" : "Direct");
}

/* 设置控制模式 */
void PID_Set_Mode(pid_controller_t* pid, uint8_t mode)
{
    uint8_t new_auto = (mode == 1);
    
    if(new_auto && pid->mode == 0)
    {
        /* 从手动切换到自动，初始化积分项 */
        pid->integral = pid->output;
        pid->last_error = 0;
        
        /* 限制积分项 */
        if(pid->integral > pid->integral_max)
            pid->integral = pid->integral_max;
        else if(pid->integral < pid->integral_min)
            pid->integral = pid->integral_min;
    }
    
    pid->mode = mode;
    printf("PID Mode: %s\\r\\n", mode ? "Auto" : "Manual");
}

/* 复位PID控制器 */
void PID_Reset(pid_controller_t* pid)
{
    pid->integral = 0.0f;
    pid->last_error = 0.0f;
    pid->derivative = 0.0f;
    pid->last_derivative = 0.0f;
    pid->output = 0.0f;
    pid->last_time = HAL_GetTick();
    
    printf("PID Controller Reset\\r\\n");
}

/* 获取PID状态 */
void PID_Get_Status(pid_controller_t* pid, char* status_str, size_t max_len)
{
    snprintf(status_str, max_len,
             "SP:%.2f PV:%.2f OUT:%.2f ERR:%.2f P:%.2f I:%.2f D:%.2f",
             pid->setpoint, pid->input, pid->output, pid->error,
             pid->Kp * pid->error,
             pid->Ki * pid->integral,
             pid->Kd * pid->derivative);
}
```

#### 温度控制应用

```c
#include "temperature_control.h"

/* 温度控制器结构体 */
typedef struct {
    pid_controller_t pid;       // PID控制器
    float current_temp;         // 当前温度
    float target_temp;          // 目标温度
    uint16_t heater_pwm;        // 加热器PWM输出
    uint8_t control_enable;     // 控制使能
    uint32_t update_time;       // 更新时间
} temperature_controller_t;

temperature_controller_t temp_ctrl;

/* 温度控制器初始化 */
void Temperature_Control_Init(void)
{
    /* 初始化PID控制器 */
    PID_Init(&temp_ctrl.pid, 2.0f, 0.5f, 0.1f);  // 初始PID参数
    PID_Set_Output_Limits(&temp_ctrl.pid, 0.0f, 1000.0f);  // PWM输出范围
    PID_Set_Sample_Time(&temp_ctrl.pid, 200);  // 200ms采样周期
    
    /* 初始化控制参数 */
    temp_ctrl.current_temp = 25.0f;     // 室温
    temp_ctrl.target_temp = 50.0f;      // 默认目标温度
    temp_ctrl.heater_pwm = 0;
    temp_ctrl.control_enable = 0;
    temp_ctrl.update_time = HAL_GetTick();
    
    /* 启动PWM */
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    
    printf("Temperature Control Initialized\\r\\n");
    printf("Target: %.1f°C, PID: Kp=%.2f Ki=%.2f Kd=%.2f\\r\\n", 
           temp_ctrl.target_temp, temp_ctrl.pid.Kp, temp_ctrl.pid.Ki, temp_ctrl.pid.Kd);
}

/* 读取温度传感器 */
float Temperature_Read_Sensor(void)
{
    /* 启动ADC转换 */
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 100);
    uint16_t adc_value = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    
    /* 转换为温度值 (假设使用NTC热敏电阻) */
    float voltage = adc_value * 3.3f / 4096.0f;
    
    /* 简化的温度转换公式 (实际应用需要根据传感器特性曲线) */
    float temperature = (voltage - 0.5f) * 100.0f;  // 假设线性关系
    
    /* 滑动平均滤波 */
    static float temp_history[5] = {25.0f, 25.0f, 25.0f, 25.0f, 25.0f};
    static uint8_t index = 0;
    
    temp_history[index] = temperature;
    index = (index + 1) % 5;
    
    float avg_temp = 0;
    for(int i = 0; i < 5; i++)
    {
        avg_temp += temp_history[i];
    }
    avg_temp /= 5.0f;
    
    return avg_temp;
}

/* 设置加热器PWM */
void Temperature_Set_Heater_PWM(uint16_t pwm_value)
{
    if(pwm_value > 1000) pwm_value = 1000;
    
    temp_ctrl.heater_pwm = pwm_value;
    
    /* 更新PWM占空比 */
    uint16_t ccr_value = pwm_value * (htim1.Init.Period + 1) / 1000;
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ccr_value);
}

/* 温度控制更新 */
void Temperature_Control_Update(void)
{
    uint32_t current_time = HAL_GetTick();
    
    if(current_time - temp_ctrl.update_time >= 200)  // 200ms更新周期
    {
        /* 读取当前温度 */
        temp_ctrl.current_temp = Temperature_Read_Sensor();
        
        if(temp_ctrl.control_enable)
        {
            /* PID计算 */
            float pid_output = PID_Compute_Position(&temp_ctrl.pid, 
                                                  temp_ctrl.target_temp, 
                                                  temp_ctrl.current_temp);
            
            /* 设置加热器输出 */
            Temperature_Set_Heater_PWM((uint16_t)pid_output);
            
            /* 输出控制信息 */
            if(current_time % 2000 < 200)  // 每2秒输出一次
            {
                char status[128];
                PID_Get_Status(&temp_ctrl.pid, status, sizeof(status));
                printf("Temp Control: %s PWM:%d\\r\\n", status, temp_ctrl.heater_pwm);
            }
        }
        else
        {
            /* 控制关闭时，关闭加热器 */
            Temperature_Set_Heater_PWM(0);
        }
        
        temp_ctrl.update_time = current_time;
    }
}

/* 设置目标温度 */
void Temperature_Set_Target(float target)
{
    if(target < 0 || target > 100) return;  // 温度范围限制
    
    temp_ctrl.target_temp = target;
    printf("Temperature Target Set: %.1f°C\\r\\n", target);
}

/* 使能/禁用控制 */
void Temperature_Control_Enable(uint8_t enable)
{
    temp_ctrl.control_enable = enable;
    
    if(enable)
    {
        PID_Reset(&temp_ctrl.pid);  // 重置PID状态
        printf("Temperature Control Enabled\\r\\n");
    }
    else
    {
        Temperature_Set_Heater_PWM(0);  // 关闭加热器
        printf("Temperature Control Disabled\\r\\n");
    }
}

/* 调整PID参数 */
void Temperature_Tune_PID(float kp, float ki, float kd)
{
    PID_Set_Tunings(&temp_ctrl.pid, kp, ki, kd);
    printf("Temperature PID Tuned: Kp=%.3f Ki=%.3f Kd=%.3f\\r\\n", kp, ki, kd);
}

/* 获取温度控制状态 */
temperature_controller_t* Temperature_Get_Status(void)
{
    return &temp_ctrl;
}
```

#### 自动整定算法

```c
#include "auto_tune.h"

/* 自动整定状态 */
typedef enum {
    TUNE_IDLE = 0,
    TUNE_PREPARE,
    TUNE_OSCILLATING,
    TUNE_ANALYZING,
    TUNE_COMPLETE
} tune_state_t;

/* 自动整定数据 */
typedef struct {
    tune_state_t state;         // 整定状态
    float output_amplitude;     // 输出幅值
    float setpoint_offset;      // 设定值偏移
    uint32_t start_time;        // 开始时间
    uint32_t peak_times[10];    // 波峰时间
    float peak_values[10];      // 波峰值
    uint8_t peak_count;         // 波峰计数
    float ultimate_gain;        // 临界增益
    float ultimate_period;      // 临界周期
    float tuned_kp, tuned_ki, tuned_kd;  // 整定结果
} auto_tuner_t;

auto_tuner_t tuner;

/* 自动整定初始化 */
void Auto_Tune_Init(pid_controller_t* pid, float amplitude)
{
    tuner.state = TUNE_IDLE;
    tuner.output_amplitude = amplitude;
    tuner.setpoint_offset = 0.0f;
    tuner.peak_count = 0;
    
    /* 设置PID为手动模式 */
    PID_Set_Mode(pid, 0);
    
    printf("Auto Tuner Initialized (Amplitude: %.1f)\\r\\n", amplitude);
}

/* 启动自动整定 */
void Auto_Tune_Start(pid_controller_t* pid, float setpoint)
{
    if(tuner.state != TUNE_IDLE) return;
    
    tuner.state = TUNE_PREPARE;
    tuner.start_time = HAL_GetTick();
    tuner.setpoint_offset = setpoint;
    tuner.peak_count = 0;
    
    printf("Auto Tuning Started (Setpoint: %.1f)\\r\\n", setpoint);
}

/* 自动整定更新 */
void Auto_Tune_Update(pid_controller_t* pid, float input)
{
    static uint8_t output_state = 0;  // 0=低, 1=高
    static uint32_t state_time = 0;
    static float last_input = 0;
    static uint8_t rising_edge = 0;
    
    uint32_t current_time = HAL_GetTick();
    
    switch(tuner.state)
    {
        case TUNE_PREPARE:
            /* 预备阶段：等待系统稳定 */
            pid->output = tuner.setpoint_offset;
            
            if(current_time - tuner.start_time > 5000)  // 等待5秒
            {
                tuner.state = TUNE_OSCILLATING;
                state_time = current_time;
                printf("Starting Oscillation Test...\\r\\n");
            }
            break;
            
        case TUNE_OSCILLATING:
            /* 振荡阶段：继电器控制 */
            if(input > tuner.setpoint_offset)
            {
                pid->output = tuner.setpoint_offset - tuner.output_amplitude;
                output_state = 0;
            }
            else
            {
                pid->output = tuner.setpoint_offset + tuner.output_amplitude;
                output_state = 1;
            }
            
            /* 检测波峰 */
            if(input > last_input && !rising_edge)
            {
                rising_edge = 1;  // 开始上升
            }
            else if(input < last_input && rising_edge && tuner.peak_count < 10)
            {
                /* 检测到波峰 */
                tuner.peak_times[tuner.peak_count] = current_time;
                tuner.peak_values[tuner.peak_count] = last_input;
                tuner.peak_count++;
                rising_edge = 0;
                
                printf("Peak %d detected: %.2f at %ld ms\\r\\n", 
                       tuner.peak_count, last_input, current_time);
                
                /* 收集足够数据后开始分析 */
                if(tuner.peak_count >= 6)
                {
                    tuner.state = TUNE_ANALYZING;
                    printf("Analyzing oscillation data...\\r\\n");
                }
            }
            
            /* 超时保护 */
            if(current_time - state_time > 60000)  // 60秒超时
            {
                printf("Oscillation test timeout\\r\\n");
                tuner.state = TUNE_IDLE;
                PID_Set_Mode(pid, 1);  // 恢复自动模式
            }
            break;
            
        case TUNE_ANALYZING:
            /* 分析阶段：计算整定参数 */
            Auto_Tune_Calculate_Parameters();
            tuner.state = TUNE_COMPLETE;
            break;
            
        case TUNE_COMPLETE:
            /* 完成阶段：应用整定结果 */
            PID_Set_Tunings(pid, tuner.tuned_kp, tuner.tuned_ki, tuner.tuned_kd);
            PID_Set_Mode(pid, 1);  // 恢复自动模式
            
            printf("Auto Tuning Complete:\\r\\n");
            printf("  Kp = %.3f\\r\\n", tuner.tuned_kp);
            printf("  Ki = %.3f\\r\\n", tuner.tuned_ki);
            printf("  Kd = %.3f\\r\\n", tuner.tuned_kd);
            
            tuner.state = TUNE_IDLE;
            break;
    }
    
    last_input = input;
}

/* 计算整定参数 */
void Auto_Tune_Calculate_Parameters(void)
{
    if(tuner.peak_count < 4) return;
    
    /* 计算平均周期 */
    float total_period = 0;
    uint8_t period_count = 0;
    
    for(int i = 2; i < tuner.peak_count; i += 2)
    {
        float period = (tuner.peak_times[i] - tuner.peak_times[i-2]) / 1000.0f;  // 转换为秒
        total_period += period;
        period_count++;
    }
    
    tuner.ultimate_period = total_period / period_count;
    
    /* 计算临界增益 */
    float avg_amplitude = 0;
    for(int i = 1; i < tuner.peak_count; i++)
    {
        avg_amplitude += fabs(tuner.peak_values[i] - tuner.setpoint_offset);
    }
    avg_amplitude /= (tuner.peak_count - 1);
    
    tuner.ultimate_gain = 4.0f * tuner.output_amplitude / (M_PI * avg_amplitude);
    
    /* Ziegler-Nichols整定规则 */
    tuner.tuned_kp = 0.6f * tuner.ultimate_gain;
    tuner.tuned_ki = 2.0f * tuner.tuned_kp / tuner.ultimate_period;
    tuner.tuned_kd = tuner.tuned_kp * tuner.ultimate_period / 8.0f;
    
    printf("Ultimate Gain: %.3f, Period: %.2f s\\r\\n", 
           tuner.ultimate_gain, tuner.ultimate_period);
}

/* 获取整定状态 */
auto_tuner_t* Auto_Tune_Get_Status(void)
{
    return &tuner;
}
```

#### 串口命令处理

```c
/* PID控制命令处理 */
void PID_Process_Command(char* cmd)
{
    if(strncmp(cmd, "TEMP", 4) == 0)
    {
        if(strncmp(cmd, "TEMP SET", 8) == 0)
        {
            float target;
            if(sscanf(cmd, "TEMP SET %f", &target) == 1)
            {
                Temperature_Set_Target(target);
            }
        }
        else if(strncmp(cmd, "TEMP ON", 7) == 0)
        {
            Temperature_Control_Enable(1);
        }
        else if(strncmp(cmd, "TEMP OFF", 8) == 0)
        {
            Temperature_Control_Enable(0);
        }
        else if(strncmp(cmd, "TEMP STATUS", 11) == 0)
        {
            temperature_controller_t* temp = Temperature_Get_Status();
            printf("\\r\\n=== Temperature Control ===\\r\\n");
            printf("Current: %.2f°C\\r\\n", temp->current_temp);
            printf("Target: %.2f°C\\r\\n", temp->target_temp);
            printf("PWM: %d/1000\\r\\n", temp->heater_pwm);
            printf("Control: %s\\r\\n", temp->control_enable ? "ON" : "OFF");
            
            char status[128];
            PID_Get_Status(&temp->pid, status, sizeof(status));
            printf("PID: %s\\r\\n", status);
            printf("==========================\\r\\n");
        }
    }
    else if(strncmp(cmd, "PID", 3) == 0)
    {
        if(strncmp(cmd, "PID SET", 7) == 0)
        {
            float kp, ki, kd;
            if(sscanf(cmd, "PID SET %f %f %f", &kp, &ki, &kd) == 3)
            {
                Temperature_Tune_PID(kp, ki, kd);
            }
        }
        else if(strncmp(cmd, "PID RESET", 9) == 0)
        {
            temperature_controller_t* temp = Temperature_Get_Status();
            PID_Reset(&temp->pid);
        }
    }
    else if(strncmp(cmd, "TUNE", 4) == 0)
    {
        if(strncmp(cmd, "TUNE START", 10) == 0)
        {
            float amplitude = 20.0f;  // 默认幅值
            sscanf(cmd, "TUNE START %f", &amplitude);
            
            temperature_controller_t* temp = Temperature_Get_Status();
            Auto_Tune_Init(&temp->pid, amplitude);
            Auto_Tune_Start(&temp->pid, temp->target_temp);
        }
        else if(strncmp(cmd, "TUNE STATUS", 11) == 0)
        {
            auto_tuner_t* tuner = Auto_Tune_Get_Status();
            printf("\\r\\n=== Auto Tuner Status ===\\r\\n");
            printf("State: %d\\r\\n", tuner->state);
            printf("Peaks: %d\\r\\n", tuner->peak_count);
            if(tuner->state == TUNE_COMPLETE)
            {
                printf("Results: Kp=%.3f Ki=%.3f Kd=%.3f\\r\\n",
                       tuner->tuned_kp, tuner->tuned_ki, tuner->tuned_kd);
            }
            printf("========================\\r\\n");
        }
    }
    else
    {
        printf("PID Commands:\\r\\n");
        printf("  TEMP SET <value> - Set target temperature\\r\\n");
        printf("  TEMP ON/OFF - Enable/disable control\\r\\n");
        printf("  TEMP STATUS - Show temperature status\\r\\n");
        printf("  PID SET <kp> <ki> <kd> - Set PID parameters\\r\\n");
        printf("  PID RESET - Reset PID controller\\r\\n");
        printf("  TUNE START [amplitude] - Start auto tuning\\r\\n");
        printf("  TUNE STATUS - Show tuning status\\r\\n");
    }
}
```

## API参考

### PID控制函数

```c
/* PID控制器 */
void PID_Init(pid_controller_t* pid, float kp, float ki, float kd);
float PID_Compute_Position(pid_controller_t* pid, float setpoint, float input);
float PID_Compute_Incremental(pid_controller_t* pid, float setpoint, float input);
void PID_Set_Tunings(pid_controller_t* pid, float kp, float ki, float kd);
void PID_Set_Output_Limits(pid_controller_t* pid, float min, float max);
void PID_Set_Sample_Time(pid_controller_t* pid, uint32_t new_sample_time);
void PID_Set_Mode(pid_controller_t* pid, uint8_t mode);
void PID_Reset(pid_controller_t* pid);
```

### 温度控制函数

```c
/* 温度控制 */
void Temperature_Control_Init(void);                   // 温度控制初始化
void Temperature_Control_Update(void);                 // 控制更新
void Temperature_Set_Target(float target);             // 设置目标温度
void Temperature_Control_Enable(uint8_t enable);       // 使能控制
void Temperature_Tune_PID(float kp, float ki, float kd);
float Temperature_Read_Sensor(void);                   // 读取温度
temperature_controller_t* Temperature_Get_Status(void);
```

### 自动整定函数

```c
/* 自动整定 */
void Auto_Tune_Init(pid_controller_t* pid, float amplitude);
void Auto_Tune_Start(pid_controller_t* pid, float setpoint);
void Auto_Tune_Update(pid_controller_t* pid, float input);
void Auto_Tune_Calculate_Parameters(void);
auto_tuner_t* Auto_Tune_Get_Status(void);
```

## 配置选项

### PID参数配置

```c
/* 在pid_controller.h中配置 */
#define DEFAULT_SAMPLE_TIME     100         // 默认采样时间(ms)
#define DEFAULT_OUTPUT_MIN      0.0f        // 默认输出最小值
#define DEFAULT_OUTPUT_MAX      1000.0f     // 默认输出最大值
#define DERIVATIVE_FILTER_ALPHA 0.1f        // 微分滤波系数
```

### 温度控制配置

```c
/* 温度控制配置 */
#define TEMP_SENSOR_CHANNEL     ADC_CHANNEL_4   // 温度传感器ADC通道
#define HEATER_PWM_CHANNEL      TIM_CHANNEL_1   // 加热器PWM通道
#define TEMP_UPDATE_PERIOD      200             // 更新周期(ms)
#define TEMP_MIN_LIMIT          0.0f            // 最低温度
#define TEMP_MAX_LIMIT          100.0f          // 最高温度
```

### 自动整定配置

```c
/* 自动整定配置 */
#define TUNE_TIMEOUT_MS         60000       // 整定超时时间
#define MIN_PEAKS_REQUIRED      6           // 最少波峰数量
#define MAX_PEAKS_RECORDED      10          // 最大记录波峰数
```

## 使用场景

1. **工业控制**: 温度、压力、流量等过程控制
2. **机器人控制**: 位置、速度、力矩控制
3. **家电控制**: 空调、热水器等温度控制
4. **实验教学**: 控制理论教学和实验
5. **产品开发**: 自动控制产品的原型开发

## 故障排除

### 常见问题

1. **系统振荡**
   - 减小比例系数Kp
   - 增加微分系数Kd
   - 检查采样频率

2. **响应缓慢**
   - 增加比例系数Kp
   - 增加积分系数Ki
   - 检查执行器功率

3. **稳态误差**
   - 增加积分系数Ki
   - 检查积分饱和设置
   - 验证传感器精度

## 学习要点

1. **PID原理**: 深入理解PID控制器的工作原理
2. **参数整定**: 掌握各种PID参数整定方法
3. **抗饱和处理**: 学习积分饱和的成因和解决方法
4. **滤波技术**: 了解微分项滤波的重要性
5. **实际应用**: 将理论知识应用到实际控制系统

## 版权信息

版权 (c) 2025 MCU Electronics Technology

MIT License - 详见LICENSE文件
