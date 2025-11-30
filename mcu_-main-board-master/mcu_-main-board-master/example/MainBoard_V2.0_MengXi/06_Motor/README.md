# 06_Motor - 直流电机控制示例

[![HAL](https://img.shields.io/badge/HAL-STM32F4-brightgreen.svg)](/Drivers) [![Keil](https://img.shields.io/badge/IDE-Keil_MDK-blue.svg)](https://www.keil.com/) 

## 项目简介

06_Motor是STM32F407VET6开发板的直流电机控制示例工程，演示了如何使用PWM控制直流电机的速度和方向。该项目提供了完整的电机控制方案，包括PWM调速、方向控制、速度反馈、保护功能等，适用于各种直流电机驱动应用。

## 主要特性

- **PWM调速**: 可变频率PWM信号生成，精确控制电机转速
- **双向控制**: 支持正转、反转、停止、刹车等控制模式
- **软启动**: 渐进式加速，避免启动电流冲击
- **速度控制**: 闭环速度控制，支持速度设定和反馈
- **保护功能**: 过流保护、堵转检测、温度监控
- **多种模式**: 支持开环控制、闭环控制、位置控制
- **用户界面**: 串口命令控制和状态监控
- **实时监控**: 实时显示电机状态和运行参数

## 硬件连接

### 电机驱动模块连接

| 电机驱动引脚 | STM32引脚 | 功能描述 |
|-------------|-----------|----------|
| EN (使能)   | PC6       | 电机使能控制 |
| IN1        | PC7       | 方向控制1 |
| IN2        | PC8       | 方向控制2 |
| PWM        | PA8 (TIM1_CH1) | PWM调速信号 |

### 速度反馈连接(可选)

| 编码器引脚 | STM32引脚 | 功能描述 |
|-----------|-----------|----------|
| A相       | PA0 (TIM2_CH1) | 编码器A相 |
| B相       | PA1 (TIM2_CH2) | 编码器B相 |
| VCC       | 3.3V      | 编码器电源 |
| GND       | GND       | 编码器地 |

### 电流检测连接(可选)

| 传感器引脚 | STM32引脚 | 功能描述 |
|-----------|-----------|----------|
| OUT       | PA4 (ADC1_IN4) | 电流检测输出 |
| VCC       | 3.3V      | 传感器电源 |
| GND       | GND       | 传感器地 |

## 目录结构

```
06_Motor/
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
│   ├── 06_Motor.uvprojx         # Keil工程文件
│   └── startup_stm32f407xx.s    # 启动文件
├── User/
│   ├── MyDefine.h               # 用户定义
│   ├── Scheduler.c              # 任务调度器
│   ├── Scheduler.h
│   ├── Scheduler_Task.c         # 任务实现
│   ├── Scheduler_Task.h
│   ├── App/
│   │   ├── motor_control.c      # 电机控制应用
│   │   ├── motor_control.h
│   │   ├── speed_control.c      # 速度控制算法
│   │   ├── speed_control.h
│   │   ├── motor_monitor.c      # 电机监控
│   │   └── motor_monitor.h
│   ├── Driver/
│   │   ├── motor_driver.c       # 电机驱动
│   │   ├── motor_driver.h
│   │   ├── encoder_driver.c     # 编码器驱动
│   │   └── encoder_driver.h
│   └── Module/
│       ├── pid_controller.c     # PID控制器
│       ├── pid_controller.h
│       ├── protection.c         # 保护功能
│       └── protection.h
├── 06_Motor.ioc               # CubeMX配置文件
└── keilkilll.bat              # Keil进程清理脚本
```

## 使用方法

### 快速开始

1. **硬件连接**
   - 按照连接表连接电机驱动模块
   - 连接直流电机到驱动模块输出端
   - 连接电源(注意电压匹配)

2. **编译下载**
   ```bash
   # 打开Keil工程
   双击 MDK-ARM/06_Motor.uvprojx
   # 编译工程 (F7)
   # 下载程序 (F8)
   ```

3. **运行测试**
   - 打开串口终端(115200,8,N,1)
   - 发送控制命令测试电机功能
   - 观察电机运行状态和参数

### 代码示例

#### 电机驱动实现

```c
#include "motor_driver.h"

/* 电机状态枚举 */
typedef enum {
    MOTOR_STOP = 0,
    MOTOR_FORWARD,
    MOTOR_BACKWARD,
    MOTOR_BRAKE
} motor_direction_t;

/* 电机控制结构体 */
typedef struct {
    motor_direction_t direction;    // 运行方向
    uint16_t speed;                // 速度值(0-1000)
    uint8_t enable;                // 使能状态
    uint16_t current_speed;        // 当前实际速度
    float current_ma;              // 当前电流(mA)
    uint32_t run_time;             // 运行时间
} motor_control_t;

motor_control_t motor_ctrl;

/* 电机驱动初始化 */
void Motor_Driver_Init(void)
{
    /* 初始化PWM */
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    
    /* 初始化方向控制引脚 */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);  // IN1 = 0
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);  // IN2 = 0
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);  // EN = 0
    
    /* 初始化控制结构体 */
    motor_ctrl.direction = MOTOR_STOP;
    motor_ctrl.speed = 0;
    motor_ctrl.enable = 0;
    motor_ctrl.current_speed = 0;
    motor_ctrl.current_ma = 0;
    motor_ctrl.run_time = 0;
    
    printf("Motor Driver Initialized\\r\\n");
}

/* 设置电机方向 */
void Motor_Set_Direction(motor_direction_t direction)
{
    motor_ctrl.direction = direction;
    
    switch(direction)
    {
        case MOTOR_STOP:
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);  // IN1 = 0
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);  // IN2 = 0
            break;
            
        case MOTOR_FORWARD:
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);    // IN1 = 1
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);  // IN2 = 0
            break;
            
        case MOTOR_BACKWARD:
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);  // IN1 = 0
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);    // IN2 = 1
            break;
            
        case MOTOR_BRAKE:
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);    // IN1 = 1
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);    // IN2 = 1
            break;
    }
}

/* 设置电机速度 */
void Motor_Set_Speed(uint16_t speed)
{
    if(speed > 1000) speed = 1000;  // 限制最大速度
    
    motor_ctrl.speed = speed;
    
    /* 计算PWM占空比 */
    uint16_t pwm_value = speed * (htim1.Init.Period + 1) / 1000;
    
    /* 更新PWM占空比 */
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm_value);
}

/* 电机使能控制 */
void Motor_Enable(uint8_t enable)
{
    motor_ctrl.enable = enable;
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
    
    if(enable)
    {
        motor_ctrl.run_time = HAL_GetTick();
        printf("Motor Enabled\\r\\n");
    }
    else
    {
        Motor_Set_Speed(0);
        Motor_Set_Direction(MOTOR_STOP);
        printf("Motor Disabled\\r\\n");
    }
}

/* 电机软启动 */
void Motor_Soft_Start(motor_direction_t direction, uint16_t target_speed, uint16_t ramp_time_ms)
{
    printf("Motor Soft Start: Dir=%d, Speed=%d\\r\\n", direction, target_speed);
    
    Motor_Set_Direction(direction);
    Motor_Enable(1);
    
    uint32_t start_time = HAL_GetTick();
    uint16_t current_speed = 0;
    
    while(current_speed < target_speed)
    {
        uint32_t elapsed = HAL_GetTick() - start_time;
        current_speed = (uint16_t)(target_speed * elapsed / ramp_time_ms);
        
        if(current_speed > target_speed)
            current_speed = target_speed;
            
        Motor_Set_Speed(current_speed);
        HAL_Delay(10);
    }
    
    printf("Soft Start Complete: Speed=%d\\r\\n", current_speed);
}

/* 电机急停 */
void Motor_Emergency_Stop(void)
{
    Motor_Set_Speed(0);
    Motor_Set_Direction(MOTOR_BRAKE);
    HAL_Delay(100);  // 刹车100ms
    Motor_Set_Direction(MOTOR_STOP);
    Motor_Enable(0);
    
    printf("Emergency Stop Executed\\r\\n");
}

/* 获取电机状态 */
motor_control_t* Motor_Get_Status(void)
{
    /* 更新运行时间 */
    if(motor_ctrl.enable)
    {
        motor_ctrl.run_time = HAL_GetTick() - motor_ctrl.run_time;
    }
    
    return &motor_ctrl;
}
```

#### 编码器反馈驱动

```c
#include "encoder_driver.h"

/* 编码器数据结构 */
typedef struct {
    int32_t count;          // 脉冲计数
    int32_t last_count;     // 上次计数
    float speed_rpm;        // 转速(RPM)
    uint8_t direction;      // 旋转方向
    uint32_t update_time;   // 更新时间
} encoder_data_t;

encoder_data_t encoder;

#define ENCODER_PPR         360     // 编码器每转脉冲数
#define SPEED_CALC_PERIOD   100     // 速度计算周期(ms)

/* 编码器初始化 */
void Encoder_Init(void)
{
    /* 启动编码器模式定时器 */
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    
    /* 设置定时器初始值 */
    __HAL_TIM_SET_COUNTER(&htim2, 32768);
    
    /* 初始化数据结构 */
    encoder.count = 0;
    encoder.last_count = 0;
    encoder.speed_rpm = 0;
    encoder.direction = 0;
    encoder.update_time = HAL_GetTick();
    
    printf("Encoder Initialized (PPR=%d)\\r\\n", ENCODER_PPR);
}

/* 读取编码器计数 */
int32_t Encoder_Get_Count(void)
{
    uint16_t tim_count = __HAL_TIM_GET_COUNTER(&htim2);
    encoder.count = tim_count - 32768;  // 转换为有符号数
    
    return encoder.count;
}

/* 计算转速 */
float Encoder_Get_Speed_RPM(void)
{
    uint32_t current_time = HAL_GetTick();
    
    if(current_time - encoder.update_time >= SPEED_CALC_PERIOD)
    {
        int32_t current_count = Encoder_Get_Count();
        int32_t delta_count = current_count - encoder.last_count;
        uint32_t delta_time = current_time - encoder.update_time;
        
        /* 计算转速 RPM = (脉冲差 / 编码器PPR) * (60000ms / 时间差ms) */
        encoder.speed_rpm = (float)delta_count * 60000.0f / (ENCODER_PPR * delta_time);
        
        /* 更新方向 */
        encoder.direction = (delta_count >= 0) ? 1 : 0;
        
        /* 更新记录 */
        encoder.last_count = current_count;
        encoder.update_time = current_time;
    }
    
    return encoder.speed_rpm;
}

/* 获取编码器数据 */
encoder_data_t* Encoder_Get_Data(void)
{
    Encoder_Get_Count();
    Encoder_Get_Speed_RPM();
    
    return &encoder;
}

/* 复位编码器 */
void Encoder_Reset(void)
{
    __HAL_TIM_SET_COUNTER(&htim2, 32768);
    encoder.count = 0;
    encoder.last_count = 0;
    encoder.speed_rpm = 0;
    encoder.update_time = HAL_GetTick();
    
    printf("Encoder Reset\\r\\n");
}
```

#### 闭环速度控制

```c
#include "speed_control.h"
#include "pid_controller.h"

/* 速度控制结构体 */
typedef struct {
    float target_speed;     // 目标速度(RPM)
    float current_speed;    // 当前速度(RPM)
    float output;           // 控制输出(0-1000)
    uint8_t control_mode;   // 控制模式: 0=开环, 1=闭环
    pid_controller_t pid;   // PID控制器
} speed_control_t;

speed_control_t speed_ctrl;

/* 速度控制初始化 */
void Speed_Control_Init(void)
{
    /* 初始化PID控制器 */
    PID_Init(&speed_ctrl.pid, 2.0f, 0.5f, 0.1f);  // Kp=2.0, Ki=0.5, Kd=0.1
    PID_Set_Limits(&speed_ctrl.pid, 0, 1000);      // 输出限制0-1000
    
    /* 初始化控制参数 */
    speed_ctrl.target_speed = 0;
    speed_ctrl.current_speed = 0;
    speed_ctrl.output = 0;
    speed_ctrl.control_mode = 1;  // 默认闭环控制
    
    printf("Speed Control Initialized\\r\\n");
}

/* 设置目标速度 */
void Speed_Control_Set_Target(float target_rpm)
{
    speed_ctrl.target_speed = target_rpm;
    
    /* 重置PID控制器 */
    PID_Reset(&speed_ctrl.pid);
    
    printf("Target Speed Set: %.1f RPM\\r\\n", target_rpm);
}

/* 速度控制更新 */
void Speed_Control_Update(void)
{
    /* 获取当前速度反馈 */
    encoder_data_t* encoder_data = Encoder_Get_Data();
    speed_ctrl.current_speed = fabs(encoder_data->speed_rpm);
    
    if(speed_ctrl.control_mode == 1)  // 闭环控制
    {
        /* PID计算 */
        float error = speed_ctrl.target_speed - speed_ctrl.current_speed;
        speed_ctrl.output = PID_Compute(&speed_ctrl.pid, error);
        
        /* 设置电机输出 */
        Motor_Set_Speed((uint16_t)speed_ctrl.output);
    }
    else  // 开环控制
    {
        /* 直接设置目标速度对应的PWM */
        speed_ctrl.output = speed_ctrl.target_speed * 1000.0f / 3000.0f;  // 假设最大速度3000RPM
        Motor_Set_Speed((uint16_t)speed_ctrl.output);
    }
}

/* 设置控制模式 */
void Speed_Control_Set_Mode(uint8_t mode)
{
    speed_ctrl.control_mode = mode;
    
    if(mode == 1)
    {
        PID_Reset(&speed_ctrl.pid);
        printf("Speed Control Mode: Closed Loop\\r\\n");
    }
    else
    {
        printf("Speed Control Mode: Open Loop\\r\\n");
    }
}

/* 获取控制状态 */
speed_control_t* Speed_Control_Get_Status(void)
{
    return &speed_ctrl;
}

/* 速度控制任务 */
void Speed_Control_Task(void)
{
    static uint32_t last_update = 0;
    
    if(HAL_GetTick() - last_update >= 50)  // 50ms更新周期
    {
        Speed_Control_Update();
        last_update = HAL_GetTick();
    }
}
```

#### 电流监控和保护

```c
#include "protection.h"

/* 保护参数结构体 */
typedef struct {
    float max_current_ma;       // 最大电流限制(mA)
    float current_ma;           // 当前电流(mA)
    uint32_t overcurrent_time;  // 过流持续时间
    uint32_t stall_time;        // 堵转检测时间
    uint8_t protection_active;  // 保护激活状态
    uint8_t error_code;         // 错误代码
} motor_protection_t;

motor_protection_t protection;

/* 保护功能初始化 */
void Motor_Protection_Init(void)
{
    /* 启动ADC */
    HAL_ADC_Start(&hadc1);
    
    /* 初始化保护参数 */
    protection.max_current_ma = 2000.0f;    // 最大电流2A
    protection.current_ma = 0;
    protection.overcurrent_time = 0;
    protection.stall_time = 0;
    protection.protection_active = 0;
    protection.error_code = 0;
    
    printf("Motor Protection Initialized (Max Current: %.1fA)\\r\\n", 
           protection.max_current_ma / 1000.0f);
}

/* 读取电机电流 */
float Motor_Get_Current(void)
{
    /* ADC采样 */
    HAL_ADC_PollForConversion(&hadc1, 100);
    uint16_t adc_value = HAL_ADC_GetValue(&hadc1);
    
    /* 转换为电流值 (假设电流传感器灵敏度为100mV/A, 参考电压3.3V) */
    float voltage = adc_value * 3.3f / 4096.0f;
    protection.current_ma = voltage * 1000.0f / 0.1f;  // 100mV/A = 0.1V/A
    
    return protection.current_ma;
}

/* 过流检测 */
uint8_t Motor_Check_Overcurrent(void)
{
    float current = Motor_Get_Current();
    
    if(current > protection.max_current_ma)
    {
        if(protection.overcurrent_time == 0)
        {
            protection.overcurrent_time = HAL_GetTick();
        }
        else if(HAL_GetTick() - protection.overcurrent_time > 500)  // 过流持续500ms
        {
            protection.error_code = 1;  // 过流错误
            return 1;
        }
    }
    else
    {
        protection.overcurrent_time = 0;
    }
    
    return 0;
}

/* 堵转检测 */
uint8_t Motor_Check_Stall(void)
{
    motor_control_t* motor_status = Motor_Get_Status();
    encoder_data_t* encoder_data = Encoder_Get_Data();
    
    /* 如果电机使能且有输出，但是没有速度反馈 */
    if(motor_status->enable && motor_status->speed > 200 && 
       fabs(encoder_data->speed_rpm) < 10.0f)
    {
        if(protection.stall_time == 0)
        {
            protection.stall_time = HAL_GetTick();
        }
        else if(HAL_GetTick() - protection.stall_time > 2000)  // 堵转持续2s
        {
            protection.error_code = 2;  // 堵转错误
            return 1;
        }
    }
    else
    {
        protection.stall_time = 0;
    }
    
    return 0;
}

/* 保护检测任务 */
void Motor_Protection_Task(void)
{
    /* 检查过流 */
    if(Motor_Check_Overcurrent())
    {
        Motor_Emergency_Stop();
        protection.protection_active = 1;
        printf("PROTECTION: Overcurrent detected! Current: %.1fA\\r\\n", 
               protection.current_ma / 1000.0f);
    }
    
    /* 检查堵转 */
    if(Motor_Check_Stall())
    {
        Motor_Emergency_Stop();
        protection.protection_active = 1;
        printf("PROTECTION: Motor stall detected!\\r\\n");
    }
}

/* 复位保护状态 */
void Motor_Protection_Reset(void)
{
    protection.protection_active = 0;
    protection.error_code = 0;
    protection.overcurrent_time = 0;
    protection.stall_time = 0;
    
    printf("Motor Protection Reset\\r\\n");
}

/* 获取保护状态 */
motor_protection_t* Motor_Protection_Get_Status(void)
{
    return &protection;
}
```

#### 串口控制命令

```c
/* 串口命令处理 */
void Motor_Process_Command(char* cmd)
{
    if(strncmp(cmd, "START", 5) == 0)
    {
        int speed, direction;
        if(sscanf(cmd, "START %d %d", &speed, &direction) == 2)
        {
            Motor_Set_Direction(direction);
            Motor_Soft_Start(direction, speed, 2000);  // 2秒软启动
            printf("Motor Started: Speed=%d, Dir=%d\\r\\n", speed, direction);
        }
    }
    else if(strncmp(cmd, "STOP", 4) == 0)
    {
        Motor_Emergency_Stop();
        printf("Motor Stopped\\r\\n");
    }
    else if(strncmp(cmd, "SPEED", 5) == 0)
    {
        int speed;
        if(sscanf(cmd, "SPEED %d", &speed) == 1)
        {
            if(speed >= 0 && speed <= 1000)
            {
                Motor_Set_Speed(speed);
                printf("Speed Set: %d\\r\\n", speed);
            }
            else
            {
                printf("Speed range: 0-1000\\r\\n");
            }
        }
    }
    else if(strncmp(cmd, "STATUS", 6) == 0)
    {
        motor_control_t* motor_status = Motor_Get_Status();
        encoder_data_t* encoder_data = Encoder_Get_Data();
        motor_protection_t* protection_status = Motor_Protection_Get_Status();
        
        printf("\\r\\n=== Motor Status ===\\r\\n");
        printf("Enable: %s\\r\\n", motor_status->enable ? "ON" : "OFF");
        printf("Direction: %d\\r\\n", motor_status->direction);
        printf("Speed Setting: %d\\r\\n", motor_status->speed);
        printf("Actual Speed: %.1f RPM\\r\\n", encoder_data->speed_rpm);
        printf("Current: %.1f mA\\r\\n", protection_status->current_ma);
        printf("Protection: %s\\r\\n", protection_status->protection_active ? "ACTIVE" : "OK");
        printf("==================\\r\\n");
    }
    else if(strncmp(cmd, "RESET", 5) == 0)
    {
        Motor_Protection_Reset();
        Encoder_Reset();
        printf("System Reset\\r\\n");
    }
    else
    {
        printf("Commands: START <speed> <dir>, STOP, SPEED <value>, STATUS, RESET\\r\\n");
    }
}
```

## API参考

### 电机控制函数

```c
/* 基础控制 */
void Motor_Driver_Init(void);                              // 电机驱动初始化
void Motor_Set_Direction(motor_direction_t direction);     // 设置方向
void Motor_Set_Speed(uint16_t speed);                      // 设置速度(0-1000)
void Motor_Enable(uint8_t enable);                         // 使能控制
```

### 高级控制函数

```c
/* 高级功能 */
void Motor_Soft_Start(motor_direction_t direction, uint16_t target_speed, uint16_t ramp_time_ms);
void Motor_Emergency_Stop(void);                           // 急停
motor_control_t* Motor_Get_Status(void);                   // 获取状态
```

### 编码器函数

```c
/* 编码器接口 */
void Encoder_Init(void);                                   // 编码器初始化
int32_t Encoder_Get_Count(void);                          // 获取计数
float Encoder_Get_Speed_RPM(void);                        // 获取转速
encoder_data_t* Encoder_Get_Data(void);                   // 获取数据
void Encoder_Reset(void);                                 // 复位编码器
```

### 速度控制函数

```c
/* 速度控制 */
void Speed_Control_Init(void);                            // 速度控制初始化
void Speed_Control_Set_Target(float target_rpm);          // 设置目标速度
void Speed_Control_Update(void);                          // 控制更新
void Speed_Control_Set_Mode(uint8_t mode);               // 设置控制模式
void Speed_Control_Task(void);                           // 控制任务
```

### 保护功能函数

```c
/* 保护功能 */
void Motor_Protection_Init(void);                         // 保护功能初始化
float Motor_Get_Current(void);                           // 获取电流
uint8_t Motor_Check_Overcurrent(void);                   // 过流检测
uint8_t Motor_Check_Stall(void);                        // 堵转检测
void Motor_Protection_Task(void);                        // 保护任务
void Motor_Protection_Reset(void);                       // 保护复位
```

## 配置选项

### 电机参数配置

```c
/* 在motor_driver.h中配置 */
#define MOTOR_MAX_SPEED         3000        // 最大转速(RPM)
#define PWM_FREQUENCY           1000        // PWM频率(Hz)
#define SOFT_START_TIME_MS      2000        // 软启动时间
#define BRAKE_TIME_MS           100         // 刹车时间
```

### 编码器配置

```c
/* 编码器参数 */
#define ENCODER_PPR             360         // 每转脉冲数
#define SPEED_CALC_PERIOD       100         // 速度计算周期(ms)
#define ENCODER_FILTER_TIME     10          // 滤波时间常数
```

### 保护参数配置

```c
/* 保护功能配置 */
#define MAX_CURRENT_MA          2000        // 最大电流(mA)
#define OVERCURRENT_TIME_MS     500         // 过流检测时间
#define STALL_DETECT_TIME_MS    2000        // 堵转检测时间
#define CURRENT_SENSOR_GAIN     100         // 电流传感器增益(mV/A)
```

## 使用场景

1. **机器人驱动**: 移动机器人的轮式驱动系统
2. **自动化设备**: 传送带、分拣机等工业设备
3. **家电控制**: 风扇、水泵等家用设备控制
4. **教学实验**: 控制系统原理学习和实验
5. **原型开发**: 产品原型的动力系统开发

## 故障排除

### 常见问题

1. **电机不转**
   - 检查电源连接和电压
   - 验证使能信号是否有效
   - 检查驱动模块是否正常

2. **速度不稳定**
   - 调整PID参数
   - 检查编码器连接
   - 验证负载是否过重

3. **保护频繁触发**
   - 检查电流设定值
   - 验证负载是否正常
   - 检查机械卡死情况

## 学习要点

1. **PWM控制**: 学习PWM信号生成和电机调速原理
2. **方向控制**: 掌握H桥驱动电路工作原理
3. **闭环控制**: 理解PID控制器设计和调试
4. **编码器应用**: 学习增量式编码器使用方法
5. **保护机制**: 了解电机保护功能的重要性

## 版权信息

版权 (c) 2025 MCU Electronics Technology

MIT License - 详见LICENSE文件
