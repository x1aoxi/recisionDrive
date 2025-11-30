# 13_AHT20 - 温湿度传感器示例

[![HAL](https://img.shields.io/badge/HAL-STM32F4-brightgreen.svg)](/Drivers) [![Keil](https://img.shields.io/badge/IDE-Keil_MDK-blue.svg)](https://www.keil.com/) 

## 项目简介

13_AHT20是STM32F407VET6开发板的AHT20温湿度传感器控制示例工程，演示如何使用I2C接口读取AHT20传感器的温度和湿度数据。该项目实现了传感器初始化、数据采集、校验处理、环境监测等功能，适用于气象监测、智能家居、农业物联网等应用场景。

## 主要特性

- **AHT20驱动**: 完整的AHT20温湿度传感器驱动
- **高精度测量**: 温度精度±0.3°C，湿度精度±2%RH
- **I2C通信**: 基于HAL库的I2C接口通信
- **数据校验**: CRC校验确保数据可靠性
- **自动校准**: 传感器自校准功能
- **环境监测**: 温湿度阈值报警
- **数据记录**: 历史数据存储和统计
- **多种显示**: OLED显示和串口输出

## 硬件连接

### AHT20连接
| 功能 | AHT20引脚 | STM32引脚 | 说明 |
|------|-----------|-----------|------|
| 电源 | VDD | 3.3V | 供电电源 |
| 地线 | GND | GND | 接地 |
| 时钟 | SCL | PB6 (I2C1_SCL) | I2C时钟线 |
| 数据 | SDA | PB7 (I2C1_SDA) | I2C数据线 |

### 其他连接
| 功能 | 引脚 | 说明 |
|------|------|------|
| OLED_SCL | PB8 | I2C2时钟线(显示) |
| OLED_SDA | PB9 | I2C2数据线(显示) |
| UART_TX | PA9 | 串口发送 |
| UART_RX | PA10 | 串口接收 |
| 报警LED | PC0 | 温度报警LED |
| 报警LED | PC1 | 湿度报警LED |
| 蜂鸣器 | PC2 | 报警蜂鸣器(可选) |

## 功能说明

### 传感器功能
- **温度测量**: -40°C ~ +85°C，精度±0.3°C
- **湿度测量**: 0%RH ~ 100%RH，精度±2%RH
- **快速响应**: 8秒响应时间(63% RH变化)
- **低功耗**: 典型功耗0.25μA(睡眠模式)

### 数据处理
- **CRC校验**: 8位CRC校验码验证数据完整性
- **温度补偿**: 自动温度补偿算法
- **数据滤波**: 移动平均滤波减少噪声
- **单位转换**: 支持摄氏度/华氏度切换

### 环境监测
- **阈值设置**: 可配置温湿度报警阈值
- **趋势分析**: 温湿度变化趋势判断
- **舒适度指数**: 计算环境舒适度评级
- **露点计算**: 根据温湿度计算露点温度

### 显示功能
- **实时显示**: OLED显示当前温湿度值
- **历史数据**: 显示最高/最低值记录
- **图形显示**: 温湿度变化曲线图
- **状态指示**: LED指示当前环境状态

## 目录结构

```
13_AHT20/
├── Core/
│   ├── Inc/                     # 核心头文件
│   │   ├── main.h
│   │   ├── i2c.h                # I2C配置头文件
│   │   └── tim.h                # 定时器配置
│   └── Src/                     # 核心源文件
│       ├── main.c
│       ├── i2c.c                # I2C配置源文件
│       └── tim.c                # 定时器配置
├── Drivers/
│   ├── CMSIS/                   # CMSIS库
│   └── STM32F4xx_HAL_Driver/    # HAL驱动库
├── MDK-ARM/
│   ├── 13_AHT20.uvprojx         # Keil工程文件
│   └── startup_stm32f407xx.s    # 启动文件
├── User/
│   ├── MyDefine.h               # 用户定义
│   ├── Scheduler.c              # 任务调度器
│   ├── Scheduler.h
│   ├── Scheduler_Task.c         # 任务实现
│   ├── Scheduler_Task.h
│   ├── App/
│   │   ├── aht20_app.c          # AHT20应用实现
│   │   ├── aht20_app.h          # AHT20应用头文件
│   │   ├── environment_app.c    # 环境监测应用
│   │   ├── environment_app.h
│   │   ├── display_app.c        # 显示应用
│   │   └── display_app.h
│   ├── Driver/
│   │   ├── aht20_driver.c       # AHT20底层驱动
│   │   ├── aht20_driver.h       # AHT20驱动头文件
│   │   └── aht20_crc.c          # CRC校验算法
│   └── Module/
│       ├── data_filter.c        # 数据滤波模块
│       ├── data_filter.h
│       ├── comfort_index.c      # 舒适度计算
│       └── comfort_index.h
├── 13_AHT20.ioc                # CubeMX配置文件
└── keilkilll.bat               # Keil进程清理脚本
```

## 使用方法

### 快速开始

1. **硬件连接**
   - 按照连接表连接AHT20传感器
   - 确保I2C上拉电阻正确(4.7kΩ)
   - 连接OLED显示屏和LED指示灯

2. **编译下载**
   ```bash
   # 打开Keil工程
   双击 MDK-ARM/13_AHT20.uvprojx
   # 编译工程 (F7)
   # 下载程序 (F8)
   ```

3. **观察结果**
   - OLED显示当前温湿度值
   - 串口输出详细数据信息
   - LED指示环境状态

### 代码示例

#### AHT20初始化

```c
#include "aht20_driver.h"

/* AHT20设备状态 */
aht20_status_t aht20_status = AHT20_OK;

/* AHT20初始化 */
aht20_status_t AHT20_Init(void)
{
    /* 等待传感器启动 */
    HAL_Delay(40);
    
    /* 检测传感器是否存在 */
    if(HAL_I2C_IsDeviceReady(&hi2c1, AHT20_I2C_ADDRESS, 3, 1000) != HAL_OK)
    {
        printf("AHT20 Device Not Found!\\r\\n");
        return AHT20_ERROR;
    }
    
    /* 读取状态寄存器 */
    uint8_t status_reg;
    if(AHT20_Read_Status(&status_reg) != AHT20_OK)
    {
        printf("AHT20 Read Status Failed!\\r\\n");
        return AHT20_ERROR;
    }
    
    /* 检查校准状态 */
    if((status_reg & 0x68) != 0x08)
    {
        /* 需要初始化校准 */
        if(AHT20_Soft_Reset() != AHT20_OK)
        {
            printf("AHT20 Soft Reset Failed!\\r\\n");
            return AHT20_ERROR;
        }
        
        HAL_Delay(20);
        
        /* 发送初始化命令 */
        uint8_t init_cmd[3] = {0xBE, 0x08, 0x00};
        if(HAL_I2C_Master_Transmit(&hi2c1, AHT20_I2C_ADDRESS, init_cmd, 3, 1000) != HAL_OK)
        {
            printf("AHT20 Init Command Failed!\\r\\n");
            return AHT20_ERROR;
        }
        
        HAL_Delay(10);
        
        /* 再次检查状态 */
        if(AHT20_Read_Status(&status_reg) != AHT20_OK)
        {
            return AHT20_ERROR;
        }
        
        if((status_reg & 0x68) != 0x08)
        {
            printf("AHT20 Calibration Failed!\\r\\n");
            return AHT20_ERROR;
        }
    }
    
    printf("AHT20 Initialized Successfully!\\r\\n");
    printf("Status Register: 0x%02X\\r\\n", status_reg);
    
    return AHT20_OK;
}
```

#### 温湿度数据读取

```c
/* 温湿度数据结构体 */
typedef struct {
    float temperature;      // 温度 (°C)
    float humidity;         // 湿度 (%RH)
    uint32_t timestamp;     // 时间戳
    uint8_t crc_ok;        // CRC校验结果
} aht20_data_t;

aht20_data_t aht20_data;

/* 触发测量并读取数据 */
aht20_status_t AHT20_Read_Data(aht20_data_t *data)
{
    uint8_t cmd[3] = {0xAC, 0x33, 0x00};  // 触发测量命令
    uint8_t read_data[7];
    uint8_t status;
    uint16_t retry_count = 0;
    
    /* 发送测量命令 */
    if(HAL_I2C_Master_Transmit(&hi2c1, AHT20_I2C_ADDRESS, cmd, 3, 1000) != HAL_OK)
    {
        return AHT20_ERROR;
    }
    
    /* 等待测量完成 */
    do {
        HAL_Delay(10);
        if(HAL_I2C_Master_Receive(&hi2c1, AHT20_I2C_ADDRESS, &status, 1, 1000) != HAL_OK)
        {
            return AHT20_ERROR;
        }
        retry_count++;
    } while((status & 0x80) && retry_count < 100);  // 检查忙标志
    
    if(retry_count >= 100)
    {
        return AHT20_TIMEOUT;
    }
    
    /* 读取测量数据 */
    if(HAL_I2C_Master_Receive(&hi2c1, AHT20_I2C_ADDRESS, read_data, 7, 1000) != HAL_OK)
    {
        return AHT20_ERROR;
    }
    
    /* CRC校验 */
    uint8_t crc = AHT20_Calculate_CRC(read_data, 6);
    data->crc_ok = (crc == read_data[6]);
    
    if(!data->crc_ok)
    {
        printf("AHT20 CRC Check Failed! Calculated: 0x%02X, Received: 0x%02X\\r\\n", 
               crc, read_data[6]);
        return AHT20_CRC_ERROR;
    }
    
    /* 数据解析 */
    uint32_t humidity_raw = ((uint32_t)read_data[1] << 12) | 
                           ((uint32_t)read_data[2] << 4) | 
                           ((read_data[3] & 0xF0) >> 4);
    
    uint32_t temperature_raw = (((uint32_t)read_data[3] & 0x0F) << 16) | 
                              ((uint32_t)read_data[4] << 8) | 
                              read_data[5];
    
    /* 转换为物理值 */
    data->humidity = ((float)humidity_raw * 100.0f) / 1048576.0f;
    data->temperature = ((float)temperature_raw * 200.0f) / 1048576.0f - 50.0f;
    data->timestamp = HAL_GetTick();
    
    return AHT20_OK;
}
```

#### CRC校验算法

```c
/* AHT20 CRC8校验 */
uint8_t AHT20_Calculate_CRC(uint8_t *data, uint8_t length)
{
    uint8_t crc = 0xFF;  // 初始值
    uint8_t i, j;
    
    for(i = 0; i < length; i++)
    {
        crc ^= data[i];
        for(j = 8; j > 0; j--)
        {
            if(crc & 0x80)
            {
                crc = (crc << 1) ^ 0x31;  // CRC多项式
            }
            else
            {
                crc = crc << 1;
            }
        }
    }
    
    return crc;
}
```

#### 数据滤波处理

```c
#define FILTER_BUFFER_SIZE 10

/* 移动平均滤波器 */
typedef struct {
    float buffer[FILTER_BUFFER_SIZE];
    uint8_t index;
    uint8_t filled;
    float sum;
} moving_average_filter_t;

moving_average_filter_t temp_filter, humid_filter;

/* 滤波器初始化 */
void Filter_Init(void)
{
    memset(&temp_filter, 0, sizeof(temp_filter));
    memset(&humid_filter, 0, sizeof(humid_filter));
}

/* 移动平均滤波 */
float Moving_Average_Filter(moving_average_filter_t *filter, float new_value)
{
    /* 减去最旧的值 */
    filter->sum -= filter->buffer[filter->index];
    
    /* 加入新值 */
    filter->buffer[filter->index] = new_value;
    filter->sum += new_value;
    
    /* 更新索引 */
    filter->index = (filter->index + 1) % FILTER_BUFFER_SIZE;
    
    if(!filter->filled && filter->index == 0)
    {
        filter->filled = 1;
    }
    
    /* 返回平均值 */
    uint8_t count = filter->filled ? FILTER_BUFFER_SIZE : filter->index;
    return count > 0 ? filter->sum / count : new_value;
}

/* AHT20数据处理任务 */
void AHT20_Task(void)
{
    static uint32_t last_read_time = 0;
    
    /* 每2秒读取一次数据 */
    if(HAL_GetTick() - last_read_time >= 2000)
    {
        last_read_time = HAL_GetTick();
        
        if(AHT20_Read_Data(&aht20_data) == AHT20_OK)
        {
            /* 数据滤波 */
            float filtered_temp = Moving_Average_Filter(&temp_filter, aht20_data.temperature);
            float filtered_humid = Moving_Average_Filter(&humid_filter, aht20_data.humidity);
            
            /* 更新滤波后的数据 */
            aht20_data.temperature = filtered_temp;
            aht20_data.humidity = filtered_humid;
            
            /* 环境监测处理 */
            Environment_Monitor(&aht20_data);
            
            /* 更新显示 */
            Display_Update();
            
            /* 串口输出 */
            printf("Temperature: %.2f°C, Humidity: %.2f%%RH, CRC: %s\\r\\n",
                   aht20_data.temperature, aht20_data.humidity,
                   aht20_data.crc_ok ? "OK" : "FAIL");
        }
        else
        {
            printf("AHT20 Read Data Failed!\\r\\n");
        }
    }
}
```

#### 环境监测功能

```c
/* 环境监测配置 */
typedef struct {
    float temp_max;         // 温度上限
    float temp_min;         // 温度下限
    float humid_max;        // 湿度上限
    float humid_min;        // 湿度下限
    uint8_t temp_alarm;     // 温度报警状态
    uint8_t humid_alarm;    // 湿度报警状态
} env_monitor_t;

env_monitor_t env_config = {
    .temp_max = 30.0f,
    .temp_min = 15.0f,
    .humid_max = 70.0f,
    .humid_min = 30.0f,
    .temp_alarm = 0,
    .humid_alarm = 0
};

/* 舒适度评级枚举 */
typedef enum {
    COMFORT_EXCELLENT,      // 优秀
    COMFORT_GOOD,          // 良好
    COMFORT_FAIR,          // 一般
    COMFORT_POOR           // 差
} comfort_level_t;

/* 计算舒适度指数 */
comfort_level_t Calculate_Comfort_Index(float temp, float humid)
{
    /* 舒适度计算公式 */
    float comfort_index = temp - 0.55f * (1.0f - humid / 100.0f) * (temp - 14.5f);
    
    if(comfort_index >= 18.0f && comfort_index <= 24.0f && 
       humid >= 40.0f && humid <= 60.0f)
    {
        return COMFORT_EXCELLENT;
    }
    else if(comfort_index >= 16.0f && comfort_index <= 26.0f && 
            humid >= 30.0f && humid <= 70.0f)
    {
        return COMFORT_GOOD;
    }
    else if(comfort_index >= 12.0f && comfort_index <= 28.0f && 
            humid >= 20.0f && humid <= 80.0f)
    {
        return COMFORT_FAIR;
    }
    else
    {
        return COMFORT_POOR;
    }
}

/* 计算露点温度 */
float Calculate_Dew_Point(float temp, float humid)
{
    float a = 17.27f;
    float b = 237.7f;
    float alpha = ((a * temp) / (b + temp)) + log(humid / 100.0f);
    float dew_point = (b * alpha) / (a - alpha);
    
    return dew_point;
}

/* 环境监测处理 */
void Environment_Monitor(aht20_data_t *data)
{
    static float temp_max_record = -50.0f;
    static float temp_min_record = 100.0f;
    static float humid_max_record = 0.0f;
    static float humid_min_record = 100.0f;
    
    /* 更新极值记录 */
    if(data->temperature > temp_max_record) temp_max_record = data->temperature;
    if(data->temperature < temp_min_record) temp_min_record = data->temperature;
    if(data->humidity > humid_max_record) humid_max_record = data->humidity;
    if(data->humidity < humid_min_record) humid_min_record = data->humidity;
    
    /* 温度报警检测 */
    if(data->temperature > env_config.temp_max || data->temperature < env_config.temp_min)
    {
        if(!env_config.temp_alarm)
        {
            env_config.temp_alarm = 1;
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);  // 温度报警LED
            printf("Temperature Alarm! Current: %.2f°C\\r\\n", data->temperature);
        }
    }
    else
    {
        env_config.temp_alarm = 0;
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
    }
    
    /* 湿度报警检测 */
    if(data->humidity > env_config.humid_max || data->humidity < env_config.humid_min)
    {
        if(!env_config.humid_alarm)
        {
            env_config.humid_alarm = 1;
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);  // 湿度报警LED
            printf("Humidity Alarm! Current: %.2f%%RH\\r\\n", data->humidity);
        }
    }
    else
    {
        env_config.humid_alarm = 0;
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
    }
    
    /* 计算舒适度 */
    comfort_level_t comfort = Calculate_Comfort_Index(data->temperature, data->humidity);
    
    /* 计算露点 */
    float dew_point = Calculate_Dew_Point(data->temperature, data->humidity);
    
    /* 输出环境分析 */
    const char* comfort_str[] = {"Excellent", "Good", "Fair", "Poor"};
    printf("Environment Analysis:\\r\\n");
    printf("  Comfort Level: %s\\r\\n", comfort_str[comfort]);
    printf("  Dew Point: %.2f°C\\r\\n", dew_point);
    printf("  Temp Record: %.2f°C ~ %.2f°C\\r\\n", temp_min_record, temp_max_record);
    printf("  Humid Record: %.2f%%RH ~ %.2f%%RH\\r\\n", humid_min_record, humid_max_record);
}
```

#### OLED显示功能

```c
/* OLED显示更新 */
void Display_Update(void)
{
    char str[32];
    
    OLED_Clear();
    
    /* 显示标题 */
    OLED_Show_String(0, 0, "AHT20 Sensor");
    
    /* 显示温度 */
    sprintf(str, "Temp: %+05.1f C", aht20_data.temperature);
    OLED_Show_String(0, 2, str);
    
    /* 显示湿度 */
    sprintf(str, "Humid:%05.1f%%RH", aht20_data.humidity);
    OLED_Show_String(0, 3, str);
    
    /* 显示舒适度 */
    comfort_level_t comfort = Calculate_Comfort_Index(aht20_data.temperature, aht20_data.humidity);
    const char* comfort_str[] = {"Excellent", "Good", "Fair", "Poor"};
    sprintf(str, "Comfort:%s", comfort_str[comfort]);
    OLED_Show_String(0, 4, str);
    
    /* 显示露点温度 */
    float dew_point = Calculate_Dew_Point(aht20_data.temperature, aht20_data.humidity);
    sprintf(str, "Dew: %+05.1f C", dew_point);
    OLED_Show_String(0, 5, str);
    
    /* 显示报警状态 */
    if(env_config.temp_alarm || env_config.humid_alarm)
    {
        OLED_Show_String(0, 6, "ALARM!");
    }
    else
    {
        OLED_Show_String(0, 6, "Normal");
    }
    
    /* 显示时间戳 */
    sprintf(str, "Time:%ld", aht20_data.timestamp / 1000);
    OLED_Show_String(0, 7, str);
    
    OLED_Refresh_Gram();
}
```

## API参考

### 基础函数

```c
/* 初始化函数 */
aht20_status_t AHT20_Init(void);                        // 初始化AHT20
aht20_status_t AHT20_Soft_Reset(void);                  // 软复位
aht20_status_t AHT20_Read_Status(uint8_t *status);      // 读取状态

/* 数据读取函数 */
aht20_status_t AHT20_Read_Data(aht20_data_t *data);     // 读取温湿度数据
aht20_status_t AHT20_Trigger_Measurement(void);        // 触发测量
```

### 校验和滤波函数

```c
/* CRC校验 */
uint8_t AHT20_Calculate_CRC(uint8_t *data, uint8_t length);     // 计算CRC
uint8_t AHT20_Verify_CRC(uint8_t *data, uint8_t crc);          // 验证CRC

/* 数据滤波 */
void Filter_Init(void);                                         // 滤波器初始化
float Moving_Average_Filter(moving_average_filter_t *filter, float value); // 移动平均滤波
```

### 环境分析函数

```c
/* 环境监测 */
void Environment_Monitor(aht20_data_t *data);           // 环境监测
comfort_level_t Calculate_Comfort_Index(float temp, float humid); // 舒适度计算
float Calculate_Dew_Point(float temp, float humid);    // 露点计算
void Set_Alarm_Thresholds(float temp_max, float temp_min, float humid_max, float humid_min); // 设置报警阈值
```

## 配置选项

### 传感器配置

```c
/* 在aht20_driver.h中配置 */
#define AHT20_I2C_ADDRESS       0x70    // I2C地址(7位)
#define AHT20_I2C_ADDRESS_WRITE 0x70    // 写地址
#define AHT20_I2C_ADDRESS_READ  0x71    // 读地址
#define AHT20_MEASUREMENT_TIME  80      // 测量时间(ms)
#define AHT20_SOFT_RESET_TIME   20      // 软复位时间(ms)
```

### 环境监测配置

```c
/* 默认报警阈值 */
#define DEFAULT_TEMP_MAX        30.0f   // 温度上限(°C)
#define DEFAULT_TEMP_MIN        15.0f   // 温度下限(°C)
#define DEFAULT_HUMID_MAX       70.0f   // 湿度上限(%RH)
#define DEFAULT_HUMID_MIN       30.0f   // 湿度下限(%RH)
```

## 扩展功能

### 数据记录功能

```c
#define RECORD_SIZE 100

/* 历史数据记录 */
typedef struct {
    aht20_data_t records[RECORD_SIZE];
    uint16_t index;
    uint16_t count;
} data_record_t;

data_record_t history_data;

/* 记录数据 */
void Record_Data(aht20_data_t *data)
{
    history_data.records[history_data.index] = *data;
    history_data.index = (history_data.index + 1) % RECORD_SIZE;
    
    if(history_data.count < RECORD_SIZE)
    {
        history_data.count++;
    }
}

/* 统计分析 */
void Data_Statistics(void)
{
    if(history_data.count == 0) return;
    
    float temp_sum = 0, humid_sum = 0;
    float temp_max = -100, temp_min = 100;
    float humid_max = 0, humid_min = 100;
    
    for(int i = 0; i < history_data.count; i++)
    {
        float temp = history_data.records[i].temperature;
        float humid = history_data.records[i].humidity;
        
        temp_sum += temp;
        humid_sum += humid;
        
        if(temp > temp_max) temp_max = temp;
        if(temp < temp_min) temp_min = temp;
        if(humid > humid_max) humid_max = humid;
        if(humid < humid_min) humid_min = humid;
    }
    
    printf("Statistics (last %d records):\\r\\n", history_data.count);
    printf("Temperature - Avg: %.2f°C, Max: %.2f°C, Min: %.2f°C\\r\\n",
           temp_sum / history_data.count, temp_max, temp_min);
    printf("Humidity - Avg: %.2f%%RH, Max: %.2f%%RH, Min: %.2f%%RH\\r\\n",
           humid_sum / history_data.count, humid_max, humid_min);
}
```

### 温度单位转换

```c
/* 温度单位转换 */
float Celsius_To_Fahrenheit(float celsius)
{
    return celsius * 9.0f / 5.0f + 32.0f;
}

float Celsius_To_Kelvin(float celsius)
{
    return celsius + 273.15f;
}

/* 显示不同温度单位 */
void Display_Temperature_Units(float celsius)
{
    printf("Temperature:\\r\\n");
    printf("  %.2f°C\\r\\n", celsius);
    printf("  %.2f°F\\r\\n", Celsius_To_Fahrenheit(celsius));
    printf("  %.2f K\\r\\n", Celsius_To_Kelvin(celsius));
}
```

### 环境等级评估

```c
/* 环境等级评估 */
typedef struct {
    char name[20];
    float temp_range[2];    // [min, max]
    float humid_range[2];   // [min, max]
    char description[50];
} environment_standard_t;

const environment_standard_t env_standards[] = {
    {"Office",      {20.0f, 26.0f}, {40.0f, 60.0f}, "Office environment"},
    {"Home",        {18.0f, 24.0f}, {40.0f, 70.0f}, "Home comfort"},
    {"Warehouse",   {10.0f, 30.0f}, {30.0f, 80.0f}, "Storage condition"},
    {"Server Room", {18.0f, 22.0f}, {45.0f, 55.0f}, "Data center"},
};

/* 评估环境类型匹配度 */
void Evaluate_Environment(float temp, float humid)
{
    int std_count = sizeof(env_standards) / sizeof(env_standards[0]);
    
    printf("Environment Evaluation:\\r\\n");
    for(int i = 0; i < std_count; i++)
    {
        uint8_t temp_ok = (temp >= env_standards[i].temp_range[0]) && 
                         (temp <= env_standards[i].temp_range[1]);
        uint8_t humid_ok = (humid >= env_standards[i].humid_range[0]) && 
                          (humid <= env_standards[i].humid_range[1]);
        
        if(temp_ok && humid_ok)
        {
            printf("  ✓ %s: MATCH - %s\\r\\n", 
                   env_standards[i].name, env_standards[i].description);
        }
        else
        {
            printf("  ✗ %s: NO MATCH\\r\\n", env_standards[i].name);
        }
    }
}
```

## 故障排除

### 常见问题

1. **读取数据失败**
   - 检查I2C接线是否正确
   - 确认上拉电阻(4.7kΩ)
   - 检查设备地址(0x38)

2. **CRC校验失败**
   - 检查I2C通信质量
   - 降低I2C时钟频率
   - 增加延时时间

3. **数据异常**
   - 确保传感器供电稳定
   - 避免传感器受热源影响
   - 检查环境气流

### 调试方法

1. 使用逻辑分析仪检查I2C时序
2. 通过串口监控原始数据和CRC
3. 对比标准温湿度计验证精度

## 应用场景

1. **智能家居**: 室内环境监控和控制
2. **农业大棚**: 作物生长环境监测
3. **仓储物流**: 货物存储环境管理
4. **工业控制**: 生产车间环境监测
5. **气象站**: 气象数据采集

## 学习要点

1. **I2C通信**: 掌握I2C协议和HAL库使用
2. **数据校验**: 理解CRC校验原理和实现
3. **传感器应用**: 学习环境传感器的使用方法
4. **数据处理**: 掌握数字滤波和数据分析技术
5. **环境监测**: 了解环境参数的意义和应用

## 版权信息

版权 (c) 2025 MCU Electronics Technology

MIT License - 详见LICENSE文件
