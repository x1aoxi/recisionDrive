# 12_BNO08X - BNO08X九轴传感器示例

[![HAL](https://img.shields.io/badge/HAL-STM32F4-brightgreen.svg)](/Drivers) [![Keil](https://img.shields.io/badge/IDE-Keil_MDK-blue.svg)](https://www.keil.com/) 

## 项目简介

12_BNO08X是STM32F407VET6开发板的BNO08X九轴传感器示例工程，演示了如何使用Bosch BNO08X系列传感器进行高精度的姿态测量和运动检测。该项目提供了完整的BNO08X传感器驱动和应用方案，包括四元数姿态解算、欧拉角转换、运动检测、校准功能等，适用于无人机、机器人、VR/AR设备等需要精确姿态信息的应用。

## 主要特性

- **九轴融合**: 集成3轴加速度计、3轴陀螺仪、3轴磁力计
- **硬件融合**: 内置ARM Cortex-M0+处理器进行传感器融合
- **高精度姿态**: 提供四元数、欧拉角、旋转矩阵等多种姿态表示
- **运动检测**: 步数检测、活动识别、显著运动检测
- **校准功能**: 自动校准和手动校准支持
- **多种报告**: 支持旋转向量、游戏旋转向量、重力向量等
- **低功耗模式**: 支持多种低功耗工作模式
- **实时数据**: 高频率实时姿态数据输出

## 硬件连接

### BNO08X传感器连接

| BNO08X引脚 | STM32引脚 | 功能描述 |
|-----------|-----------|----------|
| VDD | 3.3V | 传感器电源 |
| GND | GND | 电源地 |
| SDA | PB9 (I2C1_SDA) | I2C数据线 |
| SCL | PB8 (I2C1_SCL) | I2C时钟线 |
| INT | PA5 | 中断信号(可选) |
| RST | PA6 | 复位信号(可选) |
| BOOT | GND | 启动模式选择 |
| PS0 | GND | 协议选择0 |
| PS1 | 3.3V | 协议选择1 |

### 连接示意图

```
     BNO08X传感器                STM32F407VET6
    ┌─────────────┐              ┌──────────────┐
    │ VDD         │─────────────→│ 3.3V         │
    │ GND         │─────────────→│ GND          │
    │ SDA         │←────────────→│ PB9(I2C1_SDA)│
    │ SCL         │←────────────→│ PB8(I2C1_SCL)│
    │ INT         │─────────────→│ PA5          │
    │ RST         │←─────────────│ PA6          │
    │ BOOT        │─────────────→│ GND          │
    │ PS0         │─────────────→│ GND          │
    │ PS1         │─────────────→│ 3.3V         │
    └─────────────┘              └──────────────┘
```

### I2C地址配置

| PS1 | PS0 | I2C地址 | 协议 |
|-----|-----|---------|------|
| 0   | 0   | N/A     | UART |
| 0   | 1   | N/A     | SPI  |
| 1   | 0   | 0x4A    | I2C  |
| 1   | 1   | 0x4B    | I2C  |

## 目录结构

```
12_BNO08X/
├── Core/
│   ├── Inc/                     # 核心头文件
│   │   ├── main.h
│   │   ├── gpio.h
│   │   ├── usart.h
│   │   ├── i2c.h
│   │   └── it.h
│   └── Src/                     # 核心源文件
│       ├── main.c
│       ├── gpio.c
│       ├── usart.c
│       ├── i2c.c
│       └── it.c
├── Drivers/
│   ├── CMSIS/                   # CMSIS库
│   └── STM32F4xx_HAL_Driver/    # HAL驱动库
├── MDK-ARM/
│   ├── 12_BNO08X.uvprojx        # Keil工程文件
│   └── startup_stm32f407xx.s    # 启动文件
├── User/
│   ├── MyDefine.h               # 用户定义
│   ├── Scheduler.c              # 任务调度器
│   ├── Scheduler.h
│   ├── Scheduler_Task.c         # 任务实现
│   ├── Scheduler_Task.h
│   ├── App/
│   │   ├── attitude_app.c       # 姿态应用
│   │   ├── attitude_app.h
│   │   ├── motion_detect.c      # 运动检测
│   │   ├── motion_detect.h
│   │   ├── calibration_app.c    # 校准应用
│   │   └── calibration_app.h
│   ├── Driver/
│   │   ├── bno08x_driver.c      # BNO08X驱动
│   │   ├── bno08x_driver.h
│   │   ├── shtp_protocol.c      # SHTP协议
│   │   └── shtp_protocol.h
│   └── Module/
│       ├── quaternion_math.c    # 四元数数学库
│       ├── quaternion_math.h
│       ├── euler_convert.c      # 欧拉角转换
│       ├── euler_convert.h
│       ├── filter_math.c        # 滤波算法
│       └── filter_math.h
├── 12_BNO08X.ioc               # CubeMX配置文件
└── keilkilll.bat               # Keil进程清理脚本
```

## 使用方法

### 快速开始

1. **硬件连接**
   - 按照连接表连接BNO08X传感器
   - 确保电源和地线连接正确
   - 检查I2C上拉电阻(通常传感器板载)

2. **编译下载**
   ```bash
   # 打开Keil工程
   双击 MDK-ARM/12_BNO08X.uvprojx
   # 编译工程 (F7)
   # 下载程序 (F8)
   ```

3. **运行测试**
   - 打开串口终端(115200,8,N,1)
   - 观察传感器初始化过程
   - 移动传感器观察姿态数据变化

### 代码示例

#### BNO08X驱动核心

```c
#include "bno08x_driver.h"
#include "shtp_protocol.h"

/* BNO08X设备信息 */
#define BNO08X_I2C_ADDR         0x4A        // I2C地址(PS1=1, PS0=0)
#define BNO08X_CHIP_ID          0xA0        // 芯片ID

/* SHTP通道定义 */
#define SHTP_CHANNEL_COMMAND    0           // 命令通道
#define SHTP_CHANNEL_INPUT      1           // 输入报告通道
#define SHTP_CHANNEL_CONTROL    2           // 控制通道
#define SHTP_CHANNEL_GYRO       3           // 陀螺仪数据通道

/* 报告ID定义 */
#define SHTP_REPORT_ROTATION_VECTOR         0x05    // 旋转向量
#define SHTP_REPORT_GAME_ROTATION_VECTOR    0x08    // 游戏旋转向量
#define SHTP_REPORT_GYROSCOPE               0x02    // 陀螺仪
#define SHTP_REPORT_ACCELEROMETER           0x01    // 加速度计
#define SHTP_REPORT_MAGNETOMETER            0x03    // 磁力计
#define SHTP_REPORT_GRAVITY                 0x09    // 重力向量
#define SHTP_REPORT_LINEAR_ACCELERATION     0x0A    // 线性加速度
#define SHTP_REPORT_STEP_COUNTER            0x11    // 计步器

/* BNO08X数据结构 */
typedef struct {
    /* 四元数 */
    float quat_w, quat_x, quat_y, quat_z;   // 四元数
    float quat_accuracy;                     // 四元数精度
    
    /* 欧拉角 */
    float roll, pitch, yaw;                  // 欧拉角(弧度)
    float roll_deg, pitch_deg, yaw_deg;      // 欧拉角(度)
    
    /* 原始传感器数据 */
    float accel_x, accel_y, accel_z;         // 加速度(m/s²)
    float gyro_x, gyro_y, gyro_z;            // 角速度(rad/s)
    float mag_x, mag_y, mag_z;               // 磁场(µT)
    
    /* 融合数据 */
    float gravity_x, gravity_y, gravity_z;   // 重力向量
    float linear_x, linear_y, linear_z;      // 线性加速度
    
    /* 运动检测 */
    uint32_t step_count;                     // 步数
    uint8_t activity_class;                  // 活动类别
    uint8_t stability_class;                 // 稳定性分类
    
    /* 状态信息 */
    uint8_t calibration_status;              // 校准状态
    uint32_t timestamp;                      // 时间戳
    uint8_t data_ready;                      // 数据就绪标志
} bno08x_data_t;

bno08x_data_t bno08x_data;

/* SHTP数据包结构 */
typedef struct {
    uint16_t length;                         // 数据包长度
    uint8_t channel;                         // 通道号
    uint8_t sequence;                        // 序列号
    uint8_t data[256];                       // 数据负载
} shtp_packet_t;

/* BNO08X初始化 */
uint8_t BNO08X_Init(void)
{
    uint8_t chip_id;
    
    printf("BNO08X Initializing...\\r\\n");
    
    /* 复位传感器 */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_Delay(500);
    
    /* 检测设备 */
    if(HAL_I2C_IsDeviceReady(&hi2c1, BNO08X_I2C_ADDR << 1, 3, 1000) != HAL_OK)
    {
        printf("BNO08X not found at I2C address 0x%02X\\r\\n", BNO08X_I2C_ADDR);
        return 0;
    }
    
    /* 初始化SHTP协议 */
    SHTP_Init();
    
    /* 等待设备就绪 */
    HAL_Delay(1000);
    
    /* 启用旋转向量报告 */
    BNO08X_Enable_Rotation_Vector(50);  // 50ms周期 (20Hz)
    
    /* 启用其他报告 */
    BNO08X_Enable_Accelerometer(100);   // 加速度计
    BNO08X_Enable_Gyroscope(100);       // 陀螺仪
    BNO08X_Enable_Magnetometer(100);    // 磁力计
    BNO08X_Enable_Gravity(100);         // 重力向量
    
    /* 初始化数据结构 */
    memset(&bno08x_data, 0, sizeof(bno08x_data));
    
    printf("BNO08X Initialization Complete\\r\\n");
    return 1;
}

/* 启用旋转向量报告 */
void BNO08X_Enable_Rotation_Vector(uint16_t period_ms)
{
    uint8_t command[17] = {0};
    
    /* 设置特征报告命令 */
    command[0] = SHTP_REPORT_SET_FEATURE_COMMAND;
    command[1] = SHTP_REPORT_ROTATION_VECTOR;    // 报告ID
    command[2] = 0;                              // 特征标志
    command[3] = 0;
    command[4] = period_ms & 0xFF;               // 报告间隔(低字节)
    command[5] = (period_ms >> 8) & 0xFF;        // 报告间隔(高字节)
    command[6] = 0;                              // 批处理间隔
    command[7] = 0;
    command[8] = 0;                              // 传感器特定配置
    command[9] = 0;
    command[10] = 0;
    command[11] = 0;
    
    SHTP_Send_Packet(SHTP_CHANNEL_CONTROL, command, 17);
    
    printf("Rotation Vector enabled (Period: %d ms)\\r\\n", period_ms);
}

/* 启用加速度计报告 */
void BNO08X_Enable_Accelerometer(uint16_t period_ms)
{
    uint8_t command[17] = {0};
    
    command[0] = SHTP_REPORT_SET_FEATURE_COMMAND;
    command[1] = SHTP_REPORT_ACCELEROMETER;
    command[4] = period_ms & 0xFF;
    command[5] = (period_ms >> 8) & 0xFF;
    
    SHTP_Send_Packet(SHTP_CHANNEL_CONTROL, command, 17);
}

/* 启用陀螺仪报告 */
void BNO08X_Enable_Gyroscope(uint16_t period_ms)
{
    uint8_t command[17] = {0};
    
    command[0] = SHTP_REPORT_SET_FEATURE_COMMAND;
    command[1] = SHTP_REPORT_GYROSCOPE;
    command[4] = period_ms & 0xFF;
    command[5] = (period_ms >> 8) & 0xFF;
    
    SHTP_Send_Packet(SHTP_CHANNEL_CONTROL, command, 17);
}

/* 启用磁力计报告 */
void BNO08X_Enable_Magnetometer(uint16_t period_ms)
{
    uint8_t command[17] = {0};
    
    command[0] = SHTP_REPORT_SET_FEATURE_COMMAND;
    command[1] = SHTP_REPORT_MAGNETOMETER;
    command[4] = period_ms & 0xFF;
    command[5] = (period_ms >> 8) & 0xFF;
    
    SHTP_Send_Packet(SHTP_CHANNEL_CONTROL, command, 17);
}

/* 启用重力向量报告 */
void BNO08X_Enable_Gravity(uint16_t period_ms)
{
    uint8_t command[17] = {0};
    
    command[0] = SHTP_REPORT_SET_FEATURE_COMMAND;
    command[1] = SHTP_REPORT_GRAVITY;
    command[4] = period_ms & 0xFF;
    command[5] = (period_ms >> 8) & 0xFF;
    
    SHTP_Send_Packet(SHTP_CHANNEL_CONTROL, command, 17);
}

/* 数据更新处理 */
void BNO08X_Update(void)
{
    shtp_packet_t packet;
    
    /* 接收SHTP数据包 */
    if(SHTP_Receive_Packet(&packet))
    {
        /* 根据报告ID处理数据 */
        switch(packet.data[0])
        {
            case SHTP_REPORT_ROTATION_VECTOR:
                BNO08X_Parse_Rotation_Vector(&packet);
                break;
                
            case SHTP_REPORT_ACCELEROMETER:
                BNO08X_Parse_Accelerometer(&packet);
                break;
                
            case SHTP_REPORT_GYROSCOPE:
                BNO08X_Parse_Gyroscope(&packet);
                break;
                
            case SHTP_REPORT_MAGNETOMETER:
                BNO08X_Parse_Magnetometer(&packet);
                break;
                
            case SHTP_REPORT_GRAVITY:
                BNO08X_Parse_Gravity(&packet);
                break;
                
            case SHTP_REPORT_STEP_COUNTER:
                BNO08X_Parse_Step_Counter(&packet);
                break;
        }
    }
}

/* 解析旋转向量数据 */
void BNO08X_Parse_Rotation_Vector(shtp_packet_t* packet)
{
    uint16_t* data16 = (uint16_t*)&packet->data[4];  // 跳过报告ID和标志
    
    /* 提取四元数(Q14.2格式) */
    bno08x_data.quat_x = (float)((int16_t)data16[0]) / 16384.0f;
    bno08x_data.quat_y = (float)((int16_t)data16[1]) / 16384.0f;
    bno08x_data.quat_z = (float)((int16_t)data16[2]) / 16384.0f;
    bno08x_data.quat_w = (float)((int16_t)data16[3]) / 16384.0f;
    
    /* 提取精度信息 */
    bno08x_data.quat_accuracy = (float)((int16_t)data16[4]) / 16384.0f;
    
    /* 转换为欧拉角 */
    Quaternion_To_Euler(bno08x_data.quat_w, bno08x_data.quat_x, 
                       bno08x_data.quat_y, bno08x_data.quat_z,
                       &bno08x_data.roll, &bno08x_data.pitch, &bno08x_data.yaw);
    
    /* 转换为度 */
    bno08x_data.roll_deg = bno08x_data.roll * 180.0f / M_PI;
    bno08x_data.pitch_deg = bno08x_data.pitch * 180.0f / M_PI;
    bno08x_data.yaw_deg = bno08x_data.yaw * 180.0f / M_PI;
    
    bno08x_data.data_ready = 1;
}

/* 解析加速度计数据 */
void BNO08X_Parse_Accelerometer(shtp_packet_t* packet)
{
    uint16_t* data16 = (uint16_t*)&packet->data[4];
    
    /* 加速度数据(m/s², Q8.8格式) */
    bno08x_data.accel_x = (float)((int16_t)data16[0]) / 256.0f;
    bno08x_data.accel_y = (float)((int16_t)data16[1]) / 256.0f;
    bno08x_data.accel_z = (float)((int16_t)data16[2]) / 256.0f;
}

/* 解析陀螺仪数据 */
void BNO08X_Parse_Gyroscope(shtp_packet_t* packet)
{
    uint16_t* data16 = (uint16_t*)&packet->data[4];
    
    /* 角速度数据(rad/s, Q9.7格式) */
    bno08x_data.gyro_x = (float)((int16_t)data16[0]) / 128.0f;
    bno08x_data.gyro_y = (float)((int16_t)data16[1]) / 128.0f;
    bno08x_data.gyro_z = (float)((int16_t)data16[2]) / 128.0f;
}

/* 解析磁力计数据 */
void BNO08X_Parse_Magnetometer(shtp_packet_t* packet)
{
    uint16_t* data16 = (uint16_t*)&packet->data[4];
    
    /* 磁场数据(µT, Q4.12格式) */
    bno08x_data.mag_x = (float)((int16_t)data16[0]) / 4096.0f;
    bno08x_data.mag_y = (float)((int16_t)data16[1]) / 4096.0f;
    bno08x_data.mag_z = (float)((int16_t)data16[2]) / 4096.0f;
}

/* 解析重力向量数据 */
void BNO08X_Parse_Gravity(shtp_packet_t* packet)
{
    uint16_t* data16 = (uint16_t*)&packet->data[4];
    
    /* 重力向量(m/s², Q8.8格式) */
    bno08x_data.gravity_x = (float)((int16_t)data16[0]) / 256.0f;
    bno08x_data.gravity_y = (float)((int16_t)data16[1]) / 256.0f;
    bno08x_data.gravity_z = (float)((int16_t)data16[2]) / 256.0f;
}

/* 解析步数计数器 */
void BNO08X_Parse_Step_Counter(shtp_packet_t* packet)
{
    uint16_t* data16 = (uint16_t*)&packet->data[4];
    
    /* 步数(32位) */
    bno08x_data.step_count = ((uint32_t)data16[1] << 16) | data16[0];
}

/* 获取BNO08X数据 */
bno08x_data_t* BNO08X_Get_Data(void)
{
    return &bno08x_data;
}

/* 校准命令 */
void BNO08X_Calibrate(uint8_t sensor_type)
{
    uint8_t command[12] = {0};
    
    command[0] = 0xF2;          // 校准命令
    command[1] = sensor_type;   // 传感器类型
    command[2] = 0;             // 子命令
    command[3] = 0;             // 校准参数
    
    SHTP_Send_Packet(SHTP_CHANNEL_CONTROL, command, 12);
    
    printf("Calibration started for sensor type: %d\\r\\n", sensor_type);
}
```

#### SHTP协议实现

```c
#include "shtp_protocol.h"

/* SHTP协议常量 */
#define SHTP_HEADER_SIZE        4           // SHTP头大小
#define SHTP_MAX_PACKET_SIZE    260         // 最大数据包大小
#define SHTP_WAKE_TIME          10          // 唤醒时间(ms)

/* SHTP状态 */
static uint8_t shtp_sequence_number = 0;

/* SHTP初始化 */
void SHTP_Init(void)
{
    shtp_sequence_number = 0;
    printf("SHTP Protocol Initialized\\r\\n");
}

/* 发送SHTP数据包 */
uint8_t SHTP_Send_Packet(uint8_t channel, uint8_t* data, uint16_t length)
{
    uint8_t packet[SHTP_MAX_PACKET_SIZE];
    uint16_t packet_size = length + SHTP_HEADER_SIZE;
    
    if(packet_size > SHTP_MAX_PACKET_SIZE)
        return 0;
    
    /* 构建SHTP头 */
    packet[0] = packet_size & 0xFF;         // 长度低字节
    packet[1] = (packet_size >> 8) & 0xFF;  // 长度高字节
    packet[2] = channel;                    // 通道
    packet[3] = shtp_sequence_number++;     // 序列号
    
    /* 复制数据 */
    memcpy(&packet[4], data, length);
    
    /* 发送数据包 */
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, BNO08X_I2C_ADDR << 1, 
                                                       packet, packet_size, 1000);
    
    if(status == HAL_OK)
    {
        HAL_Delay(SHTP_WAKE_TIME);  // 等待设备处理
        return 1;
    }
    
    return 0;
}

/* 接收SHTP数据包 */
uint8_t SHTP_Receive_Packet(shtp_packet_t* packet)
{
    uint8_t header[SHTP_HEADER_SIZE];
    
    /* 读取SHTP头 */
    if(HAL_I2C_Master_Receive(&hi2c1, BNO08X_I2C_ADDR << 1, 
                             header, SHTP_HEADER_SIZE, 100) != HAL_OK)
    {
        return 0;
    }
    
    /* 解析头信息 */
    packet->length = header[0] | (header[1] << 8);
    packet->channel = header[2];
    packet->sequence = header[3];
    
    /* 检查数据包长度 */
    if(packet->length < SHTP_HEADER_SIZE || packet->length > SHTP_MAX_PACKET_SIZE)
    {
        return 0;
    }
    
    uint16_t data_length = packet->length - SHTP_HEADER_SIZE;
    
    if(data_length > 0)
    {
        /* 读取数据负载 */
        if(HAL_I2C_Master_Receive(&hi2c1, BNO08X_I2C_ADDR << 1, 
                                 packet->data, data_length, 1000) != HAL_OK)
        {
            return 0;
        }
    }
    
    return 1;
}
```

#### 四元数数学库

```c
#include "quaternion_math.h"

/* 四元数归一化 */
void Quaternion_Normalize(float* w, float* x, float* y, float* z)
{
    float norm = sqrt(*w * *w + *x * *x + *y * *y + *z * *z);
    
    if(norm > 0.0f)
    {
        *w /= norm;
        *x /= norm;
        *y /= norm;
        *z /= norm;
    }
}

/* 四元数转欧拉角 */
void Quaternion_To_Euler(float qw, float qx, float qy, float qz, 
                        float* roll, float* pitch, float* yaw)
{
    /* Roll (x-axis rotation) */
    float sinr_cosp = 2 * (qw * qx + qy * qz);
    float cosr_cosp = 1 - 2 * (qx * qx + qy * qy);
    *roll = atan2(sinr_cosp, cosr_cosp);
    
    /* Pitch (y-axis rotation) */
    float sinp = 2 * (qw * qy - qz * qx);
    if(fabs(sinp) >= 1)
        *pitch = copysign(M_PI / 2, sinp);  // 使用90度如果超出范围
    else
        *pitch = asin(sinp);
    
    /* Yaw (z-axis rotation) */
    float siny_cosp = 2 * (qw * qz + qx * qy);
    float cosy_cosp = 1 - 2 * (qy * qy + qz * qz);
    *yaw = atan2(siny_cosp, cosy_cosp);
}

/* 欧拉角转四元数 */
void Euler_To_Quaternion(float roll, float pitch, float yaw,
                        float* qw, float* qx, float* qy, float* qz)
{
    float cy = cos(yaw * 0.5f);
    float sy = sin(yaw * 0.5f);
    float cp = cos(pitch * 0.5f);
    float sp = sin(pitch * 0.5f);
    float cr = cos(roll * 0.5f);
    float sr = sin(roll * 0.5f);
    
    *qw = cy * cp * cr + sy * sp * sr;
    *qx = cy * cp * sr - sy * sp * cr;
    *qy = sy * cp * sr + cy * sp * cr;
    *qz = sy * cp * cr - cy * sp * sr;
}

/* 四元数转旋转矩阵 */
void Quaternion_To_Matrix(float qw, float qx, float qy, float qz, float matrix[3][3])
{
    float xx = qx * qx;
    float yy = qy * qy;
    float zz = qz * qz;
    float xy = qx * qy;
    float xz = qx * qz;
    float yz = qy * qz;
    float wx = qw * qx;
    float wy = qw * qy;
    float wz = qw * qz;
    
    matrix[0][0] = 1 - 2 * (yy + zz);
    matrix[0][1] = 2 * (xy - wz);
    matrix[0][2] = 2 * (xz + wy);
    
    matrix[1][0] = 2 * (xy + wz);
    matrix[1][1] = 1 - 2 * (xx + zz);
    matrix[1][2] = 2 * (yz - wx);
    
    matrix[2][0] = 2 * (xz - wy);
    matrix[2][1] = 2 * (yz + wx);
    matrix[2][2] = 1 - 2 * (xx + yy);
}
```

#### 姿态应用实现

```c
#include "attitude_app.h"

/* 姿态数据滤波 */
typedef struct {
    float alpha;                // 滤波系数
    float filtered_roll;        // 滤波后的横滚角
    float filtered_pitch;       // 滤波后的俯仰角
    float filtered_yaw;         // 滤波后的偏航角
    uint8_t initialized;        // 初始化标志
} attitude_filter_t;

attitude_filter_t attitude_filter = {0.1f, 0, 0, 0, 0};

/* 姿态应用初始化 */
void Attitude_App_Init(void)
{
    if(!BNO08X_Init())
    {
        printf("BNO08X initialization failed!\\r\\n");
        return;
    }
    
    attitude_filter.initialized = 0;
    printf("Attitude Application Initialized\\r\\n");
}

/* 姿态应用更新 */
void Attitude_App_Update(void)
{
    /* 更新BNO08X数据 */
    BNO08X_Update();
    
    bno08x_data_t* data = BNO08X_Get_Data();
    
    if(data->data_ready)
    {
        /* 应用低通滤波 */
        Attitude_Apply_Filter(data);
        
        /* 姿态数据处理 */
        Attitude_Process_Data(data);
        
        /* 清除数据就绪标志 */
        data->data_ready = 0;
    }
}

/* 应用姿态滤波 */
void Attitude_Apply_Filter(bno08x_data_t* data)
{
    if(!attitude_filter.initialized)
    {
        attitude_filter.filtered_roll = data->roll_deg;
        attitude_filter.filtered_pitch = data->pitch_deg;
        attitude_filter.filtered_yaw = data->yaw_deg;
        attitude_filter.initialized = 1;
    }
    else
    {
        /* 低通滤波 */
        attitude_filter.filtered_roll = attitude_filter.alpha * data->roll_deg + 
                                       (1.0f - attitude_filter.alpha) * attitude_filter.filtered_roll;
        
        attitude_filter.filtered_pitch = attitude_filter.alpha * data->pitch_deg + 
                                        (1.0f - attitude_filter.alpha) * attitude_filter.filtered_pitch;
        
        /* 处理yaw角的360度跳跃 */
        float yaw_diff = data->yaw_deg - attitude_filter.filtered_yaw;
        if(yaw_diff > 180.0f)
            yaw_diff -= 360.0f;
        else if(yaw_diff < -180.0f)
            yaw_diff += 360.0f;
        
        attitude_filter.filtered_yaw += attitude_filter.alpha * yaw_diff;
        
        /* 保持yaw角在[-180, 180]范围内 */
        if(attitude_filter.filtered_yaw > 180.0f)
            attitude_filter.filtered_yaw -= 360.0f;
        else if(attitude_filter.filtered_yaw < -180.0f)
            attitude_filter.filtered_yaw += 360.0f;
    }
}

/* 处理姿态数据 */
void Attitude_Process_Data(bno08x_data_t* data)
{
    static uint32_t last_output = 0;
    uint32_t current_time = HAL_GetTick();
    
    /* 每500ms输出一次姿态信息 */
    if(current_time - last_output >= 500)
    {
        printf("Attitude: R=%.1f P=%.1f Y=%.1f (Filtered: R=%.1f P=%.1f Y=%.1f)\\r\\n",
               data->roll_deg, data->pitch_deg, data->yaw_deg,
               attitude_filter.filtered_roll, attitude_filter.filtered_pitch, 
               attitude_filter.filtered_yaw);
        
        printf("Quat: W=%.3f X=%.3f Y=%.3f Z=%.3f (Acc: %.3f)\\r\\n",
               data->quat_w, data->quat_x, data->quat_y, data->quat_z, data->quat_accuracy);
        
        last_output = current_time;
    }
}

/* 获取滤波后的姿态 */
void Attitude_Get_Filtered(float* roll, float* pitch, float* yaw)
{
    *roll = attitude_filter.filtered_roll;
    *pitch = attitude_filter.filtered_pitch;
    *yaw = attitude_filter.filtered_yaw;
}
```

#### 串口命令处理

```c
/* BNO08X命令处理 */
void BNO08X_Process_Command(char* cmd)
{
    bno08x_data_t* data = BNO08X_Get_Data();
    
    if(strncmp(cmd, "STATUS", 6) == 0)
    {
        printf("\\r\\n=== BNO08X Status ===\\r\\n");
        printf("Quaternion: W=%.4f X=%.4f Y=%.4f Z=%.4f\\r\\n",
               data->quat_w, data->quat_x, data->quat_y, data->quat_z);
        printf("Euler(deg): Roll=%.2f Pitch=%.2f Yaw=%.2f\\r\\n",
               data->roll_deg, data->pitch_deg, data->yaw_deg);
        printf("Accel(m/s²): X=%.2f Y=%.2f Z=%.2f\\r\\n",
               data->accel_x, data->accel_y, data->accel_z);
        printf("Gyro(rad/s): X=%.3f Y=%.3f Z=%.3f\\r\\n",
               data->gyro_x, data->gyro_y, data->gyro_z);
        printf("Mag(µT): X=%.2f Y=%.2f Z=%.2f\\r\\n",
               data->mag_x, data->mag_y, data->mag_z);
        printf("Steps: %ld\\r\\n", data->step_count);
        printf("Accuracy: %.3f\\r\\n", data->quat_accuracy);
        printf("====================\\r\\n");
    }
    else if(strncmp(cmd, "CALIBRATE", 9) == 0)
    {
        int sensor_type = 0;
        if(sscanf(cmd, "CALIBRATE %d", &sensor_type) == 1)
        {
            BNO08X_Calibrate(sensor_type);
        }
        else
        {
            printf("Usage: CALIBRATE <sensor_type>\\r\\n");
            printf("  0: All, 1: Accel, 2: Gyro, 3: Mag\\r\\n");
        }
    }
    else if(strncmp(cmd, "RESET", 5) == 0)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
        HAL_Delay(100);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
        printf("BNO08X Reset\\r\\n");
    }
    else if(strncmp(cmd, "FILTER", 6) == 0)
    {
        float alpha;
        if(sscanf(cmd, "FILTER %f", &alpha) == 1)
        {
            if(alpha >= 0.0f && alpha <= 1.0f)
            {
                attitude_filter.alpha = alpha;
                printf("Filter coefficient set to: %.2f\\r\\n", alpha);
            }
            else
            {
                printf("Filter coefficient must be between 0.0 and 1.0\\r\\n");
            }
        }
        else
        {
            printf("Current filter coefficient: %.2f\\r\\n", attitude_filter.alpha);
        }
    }
    else
    {
        printf("BNO08X Commands:\\r\\n");
        printf("  STATUS - Show sensor data\\r\\n");
        printf("  CALIBRATE <type> - Start calibration\\r\\n");
        printf("  RESET - Reset sensor\\r\\n");
        printf("  FILTER [alpha] - Set/show filter coefficient\\r\\n");
    }
}
```

## API参考

### BNO08X控制函数

```c
/* 基础控制 */
uint8_t BNO08X_Init(void);                            // 初始化
void BNO08X_Update(void);                             // 数据更新
bno08x_data_t* BNO08X_Get_Data(void);                 // 获取数据
void BNO08X_Calibrate(uint8_t sensor_type);           // 校准
```

### 报告配置函数

```c
/* 报告配置 */
void BNO08X_Enable_Rotation_Vector(uint16_t period_ms);
void BNO08X_Enable_Accelerometer(uint16_t period_ms);
void BNO08X_Enable_Gyroscope(uint16_t period_ms);
void BNO08X_Enable_Magnetometer(uint16_t period_ms);
void BNO08X_Enable_Gravity(uint16_t period_ms);
```

### 数学函数

```c
/* 四元数数学 */
void Quaternion_Normalize(float* w, float* x, float* y, float* z);
void Quaternion_To_Euler(float qw, float qx, float qy, float qz, float* roll, float* pitch, float* yaw);
void Euler_To_Quaternion(float roll, float pitch, float yaw, float* qw, float* qx, float* qy, float* qz);
void Quaternion_To_Matrix(float qw, float qx, float qy, float qz, float matrix[3][3]);
```

### 姿态应用函数

```c
/* 姿态应用 */
void Attitude_App_Init(void);                         // 姿态应用初始化
void Attitude_App_Update(void);                       // 更新处理
void Attitude_Get_Filtered(float* roll, float* pitch, float* yaw);
```

## 配置选项

### 传感器配置

```c
/* 在bno08x_driver.h中配置 */
#define BNO08X_I2C_ADDR         0x4A        // I2C地址
#define DEFAULT_REPORT_INTERVAL 50          // 默认报告间隔(ms)
#define FILTER_ALPHA           0.1f         // 滤波系数
```

### SHTP协议配置

```c
/* SHTP配置 */
#define SHTP_MAX_PACKET_SIZE   260          // 最大数据包大小
#define SHTP_WAKE_TIME         10           // 唤醒时间(ms)
#define I2C_TIMEOUT            1000         // I2C超时时间
```

## 使用场景

1. **无人机控制**: 四旋翼、固定翼飞行器姿态控制
2. **机器人导航**: 移动机器人的方向和姿态检测
3. **VR/AR设备**: 头部追踪和手柄定位
4. **游戏控制器**: 体感游戏控制器
5. **运动分析**: 运动员动作分析和训练

## 故障排除

### 常见问题

1. **传感器无响应**
   - 检查I2C连接和地址设置
   - 确认电源电压正确(3.3V)
   - 验证复位信号

2. **数据不稳定**
   - 增加滤波处理
   - 检查机械振动影响
   - 进行传感器校准

3. **校准效果差**
   - 按照标准校准程序操作
   - 确保校准环境无磁场干扰
   - 多次校准取平均

## 学习要点

1. **传感器融合**: 理解多传感器数据融合原理
2. **四元数应用**: 掌握四元数在姿态表示中的优势
3. **坐标系转换**: 学习各种坐标系之间的转换
4. **滤波算法**: 了解数字滤波在传感器中的应用
5. **通信协议**: 学习SHTP等专用通信协议

## 版权信息

版权 (c) 2025 MCU Electronics Technology

MIT License - 详见LICENSE文件
