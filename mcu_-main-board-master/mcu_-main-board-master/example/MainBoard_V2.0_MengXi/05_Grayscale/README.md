# 05_Grayscale - 灰度传感器阵列示例

[![HAL](https://img.shields.io/badge/HAL-STM32F4-brightgreen.svg)](/Drivers) [![Keil](https://img.shields.io/badge/IDE-Keil_MDK-blue.svg)](https://www.keil.com/) 

## 项目简介

05_Grayscale是STM32F407VET6开发板的灰度传感器阵列控制示例工程，演示如何使用ADC采集多路灰度传感器信号进行循迹、边线检测等应用。该项目实现了多通道ADC采集、数字滤波、阈值检测、循迹算法等功能，是智能车竞赛和机器人循迹应用的重要参考案例。

## 主要特性

- **多通道ADC采集**: 支持8路灰度传感器同时采集
- **DMA传输**: 使用DMA实现高速数据采集
- **数字滤波**: 对ADC数据进行平滑滤波处理
- **阈值检测**: 自动和手动阈值设置
- **循迹算法**: 基于灰度值的循迹控制算法
- **实时监控**: 通过OLED显示和串口输出监控传感器状态
- **校准功能**: 支持传感器零点和满幅度校准

## 硬件连接

### 灰度传感器连接
| 传感器 | ADC通道 | STM32引脚 | 说明 |
|--------|---------|-----------|------|
| 灰度1 | ADC1_CH0 | PA0 | 左侧传感器1 |
| 灰度2 | ADC1_CH1 | PA1 | 左侧传感器2 |
| 灰度3 | ADC1_CH2 | PA2 | 左侧传感器3 |
| 灰度4 | ADC1_CH3 | PA3 | 左侧传感器4 |
| 灰度5 | ADC1_CH4 | PA4 | 右侧传感器4 |
| 灰度6 | ADC1_CH5 | PA5 | 右侧传感器3 |
| 灰度7 | ADC1_CH6 | PA6 | 右侧传感器2 |
| 灰度8 | ADC1_CH7 | PA7 | 右侧传感器1 |

### 其他连接
| 功能 | 引脚 | 说明 |
|------|------|------|
| OLED_SCL | PB6 | I2C时钟线 |
| OLED_SDA | PB7 | I2C数据线 |
| UART_TX | PA9 | 串口发送 |
| UART_RX | PA10 | 串口接收 |
| LED指示 | PC0-PC3 | 状态指示LED |

## 功能说明

### ADC采集功能
- **多通道采集**: 8路ADC通道同时采集
- **DMA传输**: 减少CPU占用，提高采集效率
- **采样率配置**: 可调整的采样频率
- **数据缓存**: 双缓冲机制保证数据完整性

### 数据处理功能
- **滑动平均滤波**: 消除ADC采集噪声
- **阈值判断**: 黑白线检测
- **边沿检测**: 检测黑白边界位置
- **位置计算**: 计算线条中心位置

### 循迹算法
- **PID控制**: 基于位置偏差的PID调节
- **模糊控制**: 多传感器融合的模糊决策
- **状态机控制**: 处理十字、弯道等特殊情况

### 显示与调试
- **OLED实时显示**: 显示传感器数值和状态
- **串口数据输出**: 便于上位机调试分析
- **LED状态指示**: 直观显示工作状态

## 目录结构

```
05_Grayscale/
├── Core/
│   ├── Inc/                     # 核心头文件
│   │   ├── main.h
│   │   ├── adc.h                # ADC配置头文件
│   │   └── dma.h                # DMA配置头文件
│   └── Src/                     # 核心源文件
│       ├── main.c
│       ├── adc.c                # ADC配置源文件
│       └── dma.c                # DMA配置源文件
├── Drivers/
│   ├── CMSIS/                   # CMSIS库
│   └── STM32F4xx_HAL_Driver/    # HAL驱动库
├── MDK-ARM/
│   ├── 05_Grayscale.uvprojx     # Keil工程文件
│   └── startup_stm32f407xx.s    # 启动文件
├── User/
│   ├── MyDefine.h               # 用户定义
│   ├── Scheduler.c              # 任务调度器
│   ├── Scheduler.h
│   ├── Scheduler_Task.c         # 任务实现
│   ├── Scheduler_Task.h
│   ├── App/
│   │   ├── grayscale_app.c      # 灰度传感器应用
│   │   ├── grayscale_app.h
│   │   ├── track_algorithm.c    # 循迹算法实现
│   │   ├── track_algorithm.h
│   │   ├── oled_app.c           # OLED显示应用
│   │   └── oled_app.h
│   ├── Driver/
│   │   ├── grayscale_driver.c   # 灰度传感器底层驱动
│   │   ├── grayscale_driver.h
│   │   └── filter.c             # 数字滤波器
│   └── Module/
├── 05_Grayscale.ioc            # CubeMX配置文件
└── keilkilll.bat               # Keil进程清理脚本
```

## 使用方法

### 快速开始

1. **硬件准备**
   - 安装8路灰度传感器阵列
   - 连接OLED显示屏
   - 准备黑白对比度明显的测试轨道

2. **编译下载**
   ```bash
   # 打开Keil工程
   双击 MDK-ARM/05_Grayscale.uvprojx
   # 编译工程 (F7)
   # 下载程序 (F8)
   ```

3. **校准传感器**
   - 将传感器阵列置于白色背景上记录基准值
   - 将传感器阵列置于黑线上记录目标值
   - 系统自动计算阈值

4. **测试循迹**
   - 将设备置于轨道上
   - 观察OLED显示的传感器数值
   - 验证循迹算法效果

### 代码示例

#### ADC和DMA初始化

```c
#include "grayscale_driver.h"
#include "adc.h"
#include "dma.h"

/* ADC采样缓冲区 */
uint16_t adc_buffer[GRAYSCALE_SENSOR_COUNT * ADC_SAMPLE_COUNT];
uint16_t grayscale_values[GRAYSCALE_SENSOR_COUNT];

/* 灰度传感器初始化 */
void Grayscale_Init(void)
{
    /* 启动ADC DMA采集 */
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, 
                      GRAYSCALE_SENSOR_COUNT * ADC_SAMPLE_COUNT);
    
    /* 初始化滤波器 */
    Filter_Init();
    
    printf("Grayscale Sensor Array Initialized\\r\\n");
    printf("Sensor Count: %d\\r\\n", GRAYSCALE_SENSOR_COUNT);
    printf("Sample Rate: %d Hz\\r\\n", ADC_SAMPLE_RATE);
}
```

#### ADC数据处理

```c
/* ADC转换完成回调 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1)
    {
        /* 数据处理标志 */
        adc_data_ready = 1;
    }
}

/* 灰度数据处理任务 */
void Grayscale_Task(void)
{
    if(adc_data_ready)
    {
        adc_data_ready = 0;
        
        /* 多次采样取平均值 */
        for(int i = 0; i < GRAYSCALE_SENSOR_COUNT; i++)
        {
            uint32_t sum = 0;
            for(int j = 0; j < ADC_SAMPLE_COUNT; j++)
            {
                sum += adc_buffer[i * ADC_SAMPLE_COUNT + j];
            }
            
            /* 原始ADC值 */
            uint16_t raw_value = sum / ADC_SAMPLE_COUNT;
            
            /* 滑动平均滤波 */
            grayscale_values[i] = Moving_Average_Filter(i, raw_value);
        }
        
        /* 数据处理完成，进行循迹计算 */
        Track_Algorithm_Update();
        
        /* 更新显示 */
        Display_Update();
    }
}
```

#### 数字滤波器实现

```c
#define FILTER_SIZE 8

/* 滑动平均滤波器 */
uint16_t Moving_Average_Filter(uint8_t channel, uint16_t new_value)
{
    static uint16_t filter_buffer[GRAYSCALE_SENSOR_COUNT][FILTER_SIZE];
    static uint8_t filter_index[GRAYSCALE_SENSOR_COUNT] = {0};
    static uint32_t filter_sum[GRAYSCALE_SENSOR_COUNT] = {0};
    
    /* 减去最旧的值 */
    filter_sum[channel] -= filter_buffer[channel][filter_index[channel]];
    
    /* 加入新的值 */
    filter_buffer[channel][filter_index[channel]] = new_value;
    filter_sum[channel] += new_value;
    
    /* 更新索引 */
    filter_index[channel] = (filter_index[channel] + 1) % FILTER_SIZE;
    
    /* 返回平均值 */
    return filter_sum[channel] / FILTER_SIZE;
}

/* 中值滤波器 */
uint16_t Median_Filter(uint8_t channel, uint16_t new_value)
{
    static uint16_t median_buffer[GRAYSCALE_SENSOR_COUNT][5];
    static uint8_t median_index[GRAYSCALE_SENSOR_COUNT] = {0};
    
    /* 存储新值 */
    median_buffer[channel][median_index[channel]] = new_value;
    median_index[channel] = (median_index[channel] + 1) % 5;
    
    /* 冒泡排序求中值 */
    uint16_t temp_buffer[5];
    memcpy(temp_buffer, median_buffer[channel], sizeof(temp_buffer));
    
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4 - i; j++)
        {
            if(temp_buffer[j] > temp_buffer[j + 1])
            {
                uint16_t temp = temp_buffer[j];
                temp_buffer[j] = temp_buffer[j + 1];
                temp_buffer[j + 1] = temp;
            }
        }
    }
    
    return temp_buffer[2]; // 返回中值
}
```

#### 循迹算法实现

```c
/* 循迹控制结构体 */
typedef struct {
    float kp;           // 比例系数
    float ki;           // 积分系数  
    float kd;           // 微分系数
    float integral;     // 积分累计值
    float last_error;   // 上次误差值
    uint16_t threshold; // 黑白阈值
    int16_t line_position; // 线条位置
} track_control_t;

track_control_t track_ctrl = {
    .kp = 0.8f,
    .ki = 0.1f,
    .kd = 0.2f,
    .threshold = 2000,
    .integral = 0.0f,
    .last_error = 0.0f,
    .line_position = 0
};

/* 计算线条位置 */
int16_t Calculate_Line_Position(void)
{
    uint8_t sensor_state = 0;
    int16_t weighted_sum = 0;
    int16_t sensor_count = 0;
    
    /* 将模拟量转换为数字量 */
    for(int i = 0; i < GRAYSCALE_SENSOR_COUNT; i++)
    {
        if(grayscale_values[i] < track_ctrl.threshold)
        {
            sensor_state |= (1 << i);
            weighted_sum += (i - 3.5f) * 1000; // 中心位置为0
            sensor_count++;
        }
    }
    
    /* 计算加权平均位置 */
    if(sensor_count > 0)
    {
        track_ctrl.line_position = weighted_sum / sensor_count;
    }
    else
    {
        /* 没有检测到线条，保持上次位置 */
    }
    
    return track_ctrl.line_position;
}

/* PID控制计算 */
float PID_Calculate(float setpoint, float current_value)
{
    float error = setpoint - current_value;
    
    /* 比例项 */
    float proportional = track_ctrl.kp * error;
    
    /* 积分项 */
    track_ctrl.integral += error;
    /* 积分限幅 */
    if(track_ctrl.integral > 100) track_ctrl.integral = 100;
    if(track_ctrl.integral < -100) track_ctrl.integral = -100;
    float integral = track_ctrl.ki * track_ctrl.integral;
    
    /* 微分项 */
    float derivative = track_ctrl.kd * (error - track_ctrl.last_error);
    track_ctrl.last_error = error;
    
    /* PID输出 */
    float output = proportional + integral + derivative;
    
    /* 输出限幅 */
    if(output > 100) output = 100;
    if(output < -100) output = -100;
    
    return output;
}

/* 循迹算法更新 */
void Track_Algorithm_Update(void)
{
    /* 计算线条位置 */
    int16_t line_pos = Calculate_Line_Position();
    
    /* PID控制计算 */
    float control_output = PID_Calculate(0, line_pos); // 目标位置为0（中心）
    
    /* 控制输出应用到电机或舵机 */
    Apply_Control_Output(control_output);
    
    /* 调试输出 */
    printf("Position: %d, Control: %.2f\\r\\n", line_pos, control_output);
}
```

#### OLED显示功能

```c
/* OLED显示更新 */
void Display_Update(void)
{
    static uint32_t update_counter = 0;
    
    /* 每100ms更新一次显示 */
    if(update_counter++ % 10 == 0)
    {
        OLED_Clear();
        
        /* 显示标题 */
        OLED_Show_String(0, 0, "Grayscale Array");
        
        /* 显示传感器数值 */
        for(int i = 0; i < 4; i++)
        {
            char str[16];
            sprintf(str, "%d:%04d", i + 1, grayscale_values[i]);
            OLED_Show_String(0, (i + 1) * 2, str);
            
            sprintf(str, "%d:%04d", i + 5, grayscale_values[i + 4]);
            OLED_Show_String(64, (i + 1) * 2, str);
        }
        
        /* 显示线条位置 */
        char pos_str[16];
        sprintf(pos_str, "Pos:%d", track_ctrl.line_position);
        OLED_Show_String(0, 6, pos_str);
        
        /* 显示阈值 */
        sprintf(pos_str, "Th:%d", track_ctrl.threshold);
        OLED_Show_String(64, 6, pos_str);
        
        /* 刷新显示 */
        OLED_Refresh_Gram();
    }
}
```

#### 校准功能

```c
/* 传感器校准 */
void Grayscale_Calibration(void)
{
    uint16_t white_values[GRAYSCALE_SENSOR_COUNT];
    uint16_t black_values[GRAYSCALE_SENSOR_COUNT];
    
    printf("开始校准，请将传感器置于白色表面...\\r\\n");
    HAL_Delay(3000);
    
    /* 采集白色表面数据 */
    for(int i = 0; i < GRAYSCALE_SENSOR_COUNT; i++)
    {
        white_values[i] = grayscale_values[i];
        printf("White[%d]: %d\\r\\n", i, white_values[i]);
    }
    
    printf("请将传感器置于黑线上...\\r\\n");
    HAL_Delay(3000);
    
    /* 采集黑线数据 */
    for(int i = 0; i < GRAYSCALE_SENSOR_COUNT; i++)
    {
        black_values[i] = grayscale_values[i];
        printf("Black[%d]: %d\\r\\n", i, black_values[i]);
    }
    
    /* 计算阈值 */
    uint32_t threshold_sum = 0;
    for(int i = 0; i < GRAYSCALE_SENSOR_COUNT; i++)
    {
        threshold_sum += (white_values[i] + black_values[i]) / 2;
    }
    track_ctrl.threshold = threshold_sum / GRAYSCALE_SENSOR_COUNT;
    
    printf("校准完成，阈值: %d\\r\\n", track_ctrl.threshold);
}
```

## API参考

### 基础函数

```c
/* 初始化函数 */
void Grayscale_Init(void);                          // 灰度传感器初始化
void Grayscale_Calibration(void);                   // 传感器校准

/* 数据采集 */
void Grayscale_Task(void);                          // 数据采集任务
uint16_t Grayscale_Get_Value(uint8_t channel);      // 获取指定通道值
void Grayscale_Get_All_Values(uint16_t *values);    // 获取所有通道值
```

### 滤波函数

```c
/* 数字滤波器 */
uint16_t Moving_Average_Filter(uint8_t channel, uint16_t new_value);  // 滑动平均滤波
uint16_t Median_Filter(uint8_t channel, uint16_t new_value);          // 中值滤波
void Filter_Init(void);                                               // 滤波器初始化
```

### 循迹算法

```c
/* 循迹控制 */
int16_t Calculate_Line_Position(void);              // 计算线条位置
float PID_Calculate(float setpoint, float current); // PID控制计算
void Track_Algorithm_Update(void);                  // 循迹算法更新
void Set_PID_Parameters(float kp, float ki, float kd); // 设置PID参数
```

## 配置选项

### 硬件配置

```c
/* 在grayscale_driver.h中配置 */
#define GRAYSCALE_SENSOR_COUNT  8       // 传感器数量
#define ADC_SAMPLE_COUNT       16       // ADC采样次数
#define ADC_SAMPLE_RATE        1000     // 采样频率(Hz)
#define DEFAULT_THRESHOLD      2000     // 默认阈值
```

### PID参数配置

```c
/* PID参数 */
#define DEFAULT_KP    0.8f      // 比例系数
#define DEFAULT_KI    0.1f      // 积分系数
#define DEFAULT_KD    0.2f      // 微分系数
```

## 扩展功能

### 自适应阈值

```c
/* 自适应阈值调整 */
void Adaptive_Threshold_Update(void)
{
    static uint16_t max_value = 0;
    static uint16_t min_value = 4095;
    
    /* 找到当前最大最小值 */
    for(int i = 0; i < GRAYSCALE_SENSOR_COUNT; i++)
    {
        if(grayscale_values[i] > max_value) max_value = grayscale_values[i];
        if(grayscale_values[i] < min_value) min_value = grayscale_values[i];
    }
    
    /* 更新阈值 */
    track_ctrl.threshold = (max_value + min_value) / 2;
}
```

### 状态机控制

```c
typedef enum {
    TRACK_NORMAL,       // 正常循迹
    TRACK_LOST,         // 线条丢失
    TRACK_CROSS,        // 十字路口
    TRACK_TURN_LEFT,    // 左转弯
    TRACK_TURN_RIGHT    // 右转弯
} track_state_t;

/* 状态机控制 */
void Track_State_Machine(void)
{
    static track_state_t current_state = TRACK_NORMAL;
    uint8_t sensor_pattern = Get_Sensor_Pattern();
    
    switch(current_state)
    {
        case TRACK_NORMAL:
            if(sensor_pattern == 0x00)
            {
                current_state = TRACK_LOST;
            }
            else if(sensor_pattern == 0xFF)
            {
                current_state = TRACK_CROSS;
            }
            break;
            
        case TRACK_LOST:
            // 搜索线条逻辑
            Search_Line();
            if(sensor_pattern != 0x00)
            {
                current_state = TRACK_NORMAL;
            }
            break;
            
        case TRACK_CROSS:
            // 十字路口处理逻辑
            Handle_Cross_Road();
            break;
            
        default:
            current_state = TRACK_NORMAL;
            break;
    }
}
```

## 故障排除

### 常见问题

1. **传感器读数异常**
   - 检查ADC参考电压
   - 确认传感器供电稳定
   - 检查接线是否正确

2. **循迹效果不佳**
   - 重新校准传感器阈值
   - 调整PID参数
   - 检查轨道对比度

3. **数据波动大**
   - 增加滤波器阶数
   - 降低采样频率
   - 改善电源滤波

### 调试方法

1. 使用串口监控传感器数值
2. 通过OLED观察实时状态
3. 调整PID参数观察效果

## 应用场景

1. **智能车竞赛**: 循迹赛道比赛
2. **AGV小车**: 自动导航车辆
3. **机器人竞赛**: 自主导航机器人
4. **工业自动化**: 传送带物品识别

## 学习要点

1. **ADC多通道采集**: 掌握多通道ADC和DMA的使用
2. **数字信号处理**: 学习数字滤波算法
3. **控制算法**: 理解PID控制原理和调参方法
4. **传感器应用**: 掌握传感器校准和数据处理
5. **实时系统**: 学习实时控制系统设计

## 版权信息

版权 (c) 2025 MCU Electronics Technology

MIT License - 详见LICENSE文件
