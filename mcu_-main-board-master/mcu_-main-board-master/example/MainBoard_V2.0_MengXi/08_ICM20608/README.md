# 08_ICM20608 - 六轴IMU传感器示例

[![HAL](https://img.shields.io/badge/HAL-STM32F4-brightgreen.svg)](/Drivers) [![Keil](https://img.shields.io/badge/IDE-Keil_MDK-blue.svg)](https://www.keil.com/) 

## 项目简介

08_ICM20608是STM32F407VET6开发板的六轴IMU传感器控制示例工程，演示如何使用ICM20608六轴惯性测量单元进行姿态检测、运动监测和数据采集。该项目实现了I2C通信、传感器配置、数据读取、姿态解算、运动检测等功能，适用于无人机、平衡车、机器人导航等应用场景。

## 主要特性

- **ICM20608驱动**: 完整的ICM20608传感器驱动程序
- **六轴数据采集**: 三轴加速度计和三轴陀螺仪数据
- **I2C通信**: 基于HAL库的I2C接口通信
- **数据滤波**: 低通滤波和卡尔曼滤波算法
- **姿态解算**: 互补滤波算法计算欧拉角
- **运动检测**: 静止、运动、碰撞检测
- **温度补偿**: 内置温度传感器数据读取
- **自校准功能**: 零点校准和偏移补偿

## 硬件连接

### ICM20608连接
| 功能 | ICM20608引脚 | STM32引脚 | 说明 |
|------|---------------|-----------|------|
| 电源 | VCC | 3.3V | 供电电源 |
| 地线 | GND | GND | 接地 |
| 时钟 | SCL | PB6 (I2C1_SCL) | I2C时钟线 |
| 数据 | SDA | PB7 (I2C1_SDA) | I2C数据线 |
| 中断 | INT | PB5 (可选) | 数据就绪中断 |

### 其他连接
| 功能 | 引脚 | 说明 |
|------|------|------|
| OLED_SCL | PB8 | I2C2时钟线(显示) |
| OLED_SDA | PB9 | I2C2数据线(显示) |
| UART_TX | PA9 | 串口发送 |
| UART_RX | PA10 | 串口接收 |
| LED指示 | PC0-PC3 | 状态指示LED |

## 功能说明

### 传感器功能
- **加速度计**: ±2g/±4g/±8g/±16g量程可选
- **陀螺仪**: ±250°/s/±500°/s/±1000°/s/±2000°/s量程可选
- **温度传感器**: 内置温度检测
- **FIFO缓冲**: 512字节硬件FIFO缓冲区

### 数据处理
- **原始数据读取**: 16位ADC数据
- **单位转换**: 转换为物理单位(m/s²、°/s、°C)
- **数据滤波**: 消除高频噪声
- **校准补偿**: 零点偏移和增益补偿

### 姿态解算
- **欧拉角计算**: Roll、Pitch、Yaw角度
- **四元数表示**: 四元数姿态表示
- **互补滤波**: 融合加速度计和陀螺仪数据
- **卡尔曼滤波**: 高精度姿态估计

### 应用功能
- **水平检测**: 检测设备是否水平
- **倾斜报警**: 倾斜角度超限报警
- **运动识别**: 静止、运动、跌落检测
- **步数计算**: 基于加速度的步数统计

## 目录结构

```
08_ICM20608/
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
│   ├── 08_ICM20608.uvprojx      # Keil工程文件
│   └── startup_stm32f407xx.s    # 启动文件
├── User/
│   ├── MyDefine.h               # 用户定义
│   ├── Scheduler.c              # 任务调度器
│   ├── Scheduler.h
│   ├── Scheduler_Task.c         # 任务实现
│   ├── Scheduler_Task.h
│   ├── App/
│   │   ├── imu_app.c            # IMU应用实现
│   │   ├── imu_app.h            # IMU应用头文件
│   │   ├── attitude_app.c       # 姿态解算应用
│   │   ├── attitude_app.h
│   │   ├── motion_app.c         # 运动检测应用
│   │   └── motion_app.h
│   ├── Driver/
│   │   ├── icm20608_driver.c    # ICM20608底层驱动
│   │   ├── icm20608_driver.h    # ICM20608驱动头文件
│   │   └── icm20608_reg.h       # ICM20608寄存器定义
│   └── Module/
│       ├── attitude_algorithm.c  # 姿态解算算法
│       ├── attitude_algorithm.h
│       ├── kalman_filter.c      # 卡尔曼滤波器
│       └── kalman_filter.h
├── 08_ICM20608.ioc             # CubeMX配置文件
└── keilkilll.bat               # Keil进程清理脚本
```

## 使用方法

### 快速开始

1. **硬件连接**
   - 按照连接表连接ICM20608传感器
   - 确保I2C上拉电阻正确(通常为4.7kΩ)
   - 连接OLED显示屏(可选)

2. **编译下载**
   ```bash
   # 打开Keil工程
   双击 MDK-ARM/08_ICM20608.uvprojx
   # 编译工程 (F7)
   # 下载程序 (F8)
   ```

3. **校准传感器**
   - 将设备保持静止水平3秒进行零点校准
   - 观察串口输出的校准结果

4. **测试功能**
   - 倾斜设备观察姿态角变化
   - 移动设备测试运动检测
   - 通过OLED或串口查看实时数据

### 代码示例

#### ICM20608初始化

```c
#include "icm20608_driver.h"

/* ICM20608设备结构体 */
icm20608_dev_t icm20608_dev;

/* ICM20608初始化 */
icm20608_status_t ICM20608_Init(void)
{
    icm20608_status_t status;
    
    /* 设备复位 */
    status = ICM20608_Reset();
    if(status != ICM20608_OK)
    {
        printf("ICM20608 Reset Failed!\\r\\n");
        return status;
    }
    
    HAL_Delay(100); // 等待复位完成
    
    /* 检测设备ID */
    uint8_t device_id;
    status = ICM20608_Read_Device_ID(&device_id);
    if(status != ICM20608_OK || device_id != ICM20608_DEVICE_ID)
    {
        printf("ICM20608 Device ID Error: 0x%02X\\r\\n", device_id);
        return ICM20608_ERROR;
    }
    
    /* 配置电源管理 */
    status = ICM20608_Set_Power_Mode(ICM20608_POWER_NORMAL);
    if(status != ICM20608_OK) return status;
    
    /* 配置加速度计 */
    status = ICM20608_Set_Accel_Range(ICM20608_ACCEL_RANGE_8G);
    if(status != ICM20608_OK) return status;
    
    status = ICM20608_Set_Accel_LPF(ICM20608_ACCEL_LPF_218HZ);
    if(status != ICM20608_OK) return status;
    
    /* 配置陀螺仪 */
    status = ICM20608_Set_Gyro_Range(ICM20608_GYRO_RANGE_1000DPS);
    if(status != ICM20608_OK) return status;
    
    status = ICM20608_Set_Gyro_LPF(ICM20608_GYRO_LPF_176HZ);
    if(status != ICM20608_OK) return status;
    
    /* 设置采样率 */
    status = ICM20608_Set_Sample_Rate(100); // 100Hz
    if(status != ICM20608_OK) return status;
    
    printf("ICM20608 Initialized Successfully!\\r\\n");
    printf("Device ID: 0x%02X\\r\\n", device_id);
    
    return ICM20608_OK;
}
```

#### 数据读取和处理

```c
/* IMU数据结构体 */
typedef struct {
    float accel_x, accel_y, accel_z;    // 加速度 (m/s²)
    float gyro_x, gyro_y, gyro_z;       // 角速度 (°/s)
    float temperature;                   // 温度 (°C)
    uint32_t timestamp;                  // 时间戳
} imu_data_t;

imu_data_t imu_data;

/* IMU数据读取任务 */
void IMU_Data_Task(void)
{
    icm20608_raw_data_t raw_data;
    
    /* 读取原始数据 */
    if(ICM20608_Read_Raw_Data(&raw_data) == ICM20608_OK)
    {
        /* 转换为物理单位 */
        imu_data.accel_x = raw_data.accel_x * ACCEL_SCALE_FACTOR;
        imu_data.accel_y = raw_data.accel_y * ACCEL_SCALE_FACTOR;
        imu_data.accel_z = raw_data.accel_z * ACCEL_SCALE_FACTOR;
        
        imu_data.gyro_x = raw_data.gyro_x * GYRO_SCALE_FACTOR;
        imu_data.gyro_y = raw_data.gyro_y * GYRO_SCALE_FACTOR;
        imu_data.gyro_z = raw_data.gyro_z * GYRO_SCALE_FACTOR;
        
        imu_data.temperature = raw_data.temperature / 326.8f + 25.0f;
        imu_data.timestamp = HAL_GetTick();
        
        /* 数据滤波 */
        IMU_Data_Filter(&imu_data);
        
        /* 姿态解算 */
        Attitude_Update(&imu_data);
        
        /* 运动检测 */
        Motion_Detection(&imu_data);
    }
}
```

#### 姿态解算算法

```c
/* 姿态角结构体 */
typedef struct {
    float roll;     // 横滚角 (°)
    float pitch;    // 俯仰角 (°)  
    float yaw;      // 航向角 (°)
} euler_angles_t;

euler_angles_t euler_angles = {0.0f, 0.0f, 0.0f};

/* 互补滤波器参数 */
#define COMPLEMENTARY_FILTER_ALPHA 0.98f
#define DT 0.01f  // 采样周期 10ms

/* 姿态更新 - 互补滤波算法 */
void Attitude_Update(imu_data_t *data)
{
    static float gyro_roll = 0.0f, gyro_pitch = 0.0f;
    
    /* 陀螺仪积分计算姿态角 */
    gyro_roll += data->gyro_x * DT;
    gyro_pitch += data->gyro_y * DT;
    
    /* 加速度计计算姿态角 */
    float accel_roll = atan2(data->accel_y, data->accel_z) * 180.0f / M_PI;
    float accel_pitch = atan2(-data->accel_x, 
                              sqrt(data->accel_y * data->accel_y + data->accel_z * data->accel_z)) 
                              * 180.0f / M_PI;
    
    /* 互补滤波融合 */
    euler_angles.roll = COMPLEMENTARY_FILTER_ALPHA * gyro_roll + 
                        (1.0f - COMPLEMENTARY_FILTER_ALPHA) * accel_roll;
    euler_angles.pitch = COMPLEMENTARY_FILTER_ALPHA * gyro_pitch + 
                         (1.0f - COMPLEMENTARY_FILTER_ALPHA) * accel_pitch;
    
    /* 航向角通过陀螺仪积分(需要磁力计修正) */
    euler_angles.yaw += data->gyro_z * DT;
    
    /* 角度范围限制 */
    if(euler_angles.roll > 180.0f) euler_angles.roll -= 360.0f;
    if(euler_angles.roll < -180.0f) euler_angles.roll += 360.0f;
    
    if(euler_angles.pitch > 180.0f) euler_angles.pitch -= 360.0f;
    if(euler_angles.pitch < -180.0f) euler_angles.pitch += 360.0f;
    
    if(euler_angles.yaw > 180.0f) euler_angles.yaw -= 360.0f;
    if(euler_angles.yaw < -180.0f) euler_angles.yaw += 360.0f;
    
    /* 更新陀螺仪积分值 */
    gyro_roll = euler_angles.roll;
    gyro_pitch = euler_angles.pitch;
}
```

#### 卡尔曼滤波器实现

```c
/* 卡尔曼滤波器结构体 */
typedef struct {
    float Q;        // 过程噪声协方差
    float R;        // 测量噪声协方差
    float P;        // 估计误差协方差
    float K;        // 卡尔曼增益
    float X;        // 状态估计值
} kalman_filter_t;

kalman_filter_t kf_roll, kf_pitch;

/* 卡尔曼滤波器初始化 */
void Kalman_Filter_Init(kalman_filter_t *kf, float Q, float R)
{
    kf->Q = Q;      // 过程噪声 (陀螺仪噪声)
    kf->R = R;      // 测量噪声 (加速度计噪声)
    kf->P = 1.0f;   // 初始估计误差
    kf->X = 0.0f;   // 初始状态
}

/* 卡尔曼滤波更新 */
float Kalman_Filter_Update(kalman_filter_t *kf, float gyro, float accel)
{
    /* 预测步骤 */
    kf->X += gyro * DT;     // 状态预测 (陀螺仪积分)
    kf->P += kf->Q;         // 协方差预测
    
    /* 更新步骤 */
    kf->K = kf->P / (kf->P + kf->R);    // 计算卡尔曼增益
    kf->X += kf->K * (accel - kf->X);   // 状态更新
    kf->P *= (1.0f - kf->K);            // 协方差更新
    
    return kf->X;
}

/* 使用卡尔曼滤波的姿态解算 */
void Attitude_Kalman_Update(imu_data_t *data)
{
    /* 计算加速度计角度 */
    float accel_roll = atan2(data->accel_y, data->accel_z) * 180.0f / M_PI;
    float accel_pitch = atan2(-data->accel_x, 
                              sqrt(data->accel_y * data->accel_y + data->accel_z * data->accel_z)) 
                              * 180.0f / M_PI;
    
    /* 卡尔曼滤波更新 */
    euler_angles.roll = Kalman_Filter_Update(&kf_roll, data->gyro_x, accel_roll);
    euler_angles.pitch = Kalman_Filter_Update(&kf_pitch, data->gyro_y, accel_pitch);
    
    /* 航向角积分 */
    euler_angles.yaw += data->gyro_z * DT;
}
```

#### 运动检测算法

```c
/* 运动状态枚举 */
typedef enum {
    MOTION_STATIC,      // 静止
    MOTION_MOVING,      // 运动
    MOTION_SHAKING,     // 震动
    MOTION_FALLING      // 跌落
} motion_state_t;

motion_state_t motion_state = MOTION_STATIC;

/* 运动检测 */
void Motion_Detection(imu_data_t *data)
{
    static float accel_magnitude_buffer[10];
    static uint8_t buffer_index = 0;
    static float accel_variance = 0.0f;
    
    /* 计算加速度幅值 */
    float magnitude = sqrt(data->accel_x * data->accel_x + 
                          data->accel_y * data->accel_y + 
                          data->accel_z * data->accel_z);
    
    /* 存入缓冲区 */
    accel_magnitude_buffer[buffer_index] = magnitude;
    buffer_index = (buffer_index + 1) % 10;
    
    /* 计算方差 */
    float mean = 0.0f;
    for(int i = 0; i < 10; i++)
    {
        mean += accel_magnitude_buffer[i];
    }
    mean /= 10.0f;
    
    float variance = 0.0f;
    for(int i = 0; i < 10; i++)
    {
        float diff = accel_magnitude_buffer[i] - mean;
        variance += diff * diff;
    }
    variance /= 10.0f;
    accel_variance = variance;
    
    /* 状态判断 */
    if(variance < 0.1f && fabs(mean - 9.8f) < 1.0f)
    {
        motion_state = MOTION_STATIC;
    }
    else if(variance > 2.0f)
    {
        motion_state = MOTION_SHAKING;
    }
    else if(mean < 5.0f)  // 自由落体检测
    {
        motion_state = MOTION_FALLING;
    }
    else
    {
        motion_state = MOTION_MOVING;
    }
}
```

#### 显示和输出

```c
/* OLED显示更新 */
void IMU_Display_Update(void)
{
    static uint32_t display_counter = 0;
    
    /* 每100ms更新一次显示 */
    if(display_counter++ % 10 == 0)
    {
        char str[32];
        
        OLED_Clear();
        OLED_Show_String(0, 0, "ICM20608 IMU");
        
        /* 显示姿态角 */
        sprintf(str, "Roll: %+06.1f", euler_angles.roll);
        OLED_Show_String(0, 2, str);
        
        sprintf(str, "Pitch:%+06.1f", euler_angles.pitch);
        OLED_Show_String(0, 3, str);
        
        sprintf(str, "Yaw:  %+06.1f", euler_angles.yaw);
        OLED_Show_String(0, 4, str);
        
        /* 显示温度 */
        sprintf(str, "Temp: %04.1fC", imu_data.temperature);
        OLED_Show_String(0, 5, str);
        
        /* 显示运动状态 */
        const char* motion_str[] = {"STATIC", "MOVING", "SHAKING", "FALLING"};
        sprintf(str, "State:%s", motion_str[motion_state]);
        OLED_Show_String(0, 6, str);
        
        OLED_Refresh_Gram();
    }
}

/* 串口数据输出 */
void IMU_Serial_Output(void)
{
    static uint32_t output_counter = 0;
    
    /* 每50ms输出一次数据 */
    if(output_counter++ % 5 == 0)
    {
        printf("IMU: Roll=%.2f, Pitch=%.2f, Yaw=%.2f, Temp=%.1f, State=%d\\r\\n",
               euler_angles.roll, euler_angles.pitch, euler_angles.yaw, 
               imu_data.temperature, motion_state);
    }
}
```

## API参考

### 基础函数

```c
/* 初始化函数 */
icm20608_status_t ICM20608_Init(void);                      // 初始化ICM20608
icm20608_status_t ICM20608_Reset(void);                     // 复位传感器
icm20608_status_t ICM20608_Read_Device_ID(uint8_t *id);     // 读取设备ID

/* 配置函数 */
icm20608_status_t ICM20608_Set_Accel_Range(icm20608_accel_range_t range);   // 设置加速度计量程
icm20608_status_t ICM20608_Set_Gyro_Range(icm20608_gyro_range_t range);     // 设置陀螺仪量程
icm20608_status_t ICM20608_Set_Sample_Rate(uint16_t rate);  // 设置采样率
```

### 数据读取函数

```c
/* 数据读取 */
icm20608_status_t ICM20608_Read_Raw_Data(icm20608_raw_data_t *data);    // 读取原始数据
icm20608_status_t ICM20608_Read_Accel(int16_t *x, int16_t *y, int16_t *z);  // 读取加速度
icm20608_status_t ICM20608_Read_Gyro(int16_t *x, int16_t *y, int16_t *z);   // 读取陀螺仪
icm20608_status_t ICM20608_Read_Temperature(int16_t *temp); // 读取温度
```

### 姿态解算函数

```c
/* 姿态解算 */
void Attitude_Update(imu_data_t *data);                     // 姿态更新
void Attitude_Kalman_Update(imu_data_t *data);             // 卡尔曼姿态更新
void Get_Euler_Angles(euler_angles_t *angles);             // 获取欧拉角
```

## 配置选项

### 传感器配置

```c
/* 在icm20608_driver.h中配置 */
#define ICM20608_I2C_ADDRESS       0xD0    // I2C地址
#define ICM20608_DEVICE_ID          0xAF    // 设备ID
#define DEFAULT_SAMPLE_RATE         100     // 默认采样率(Hz)
#define ACCEL_SCALE_FACTOR          0.00024f // 加速度缩放因子(8g量程)
#define GYRO_SCALE_FACTOR           0.030487f // 陀螺仪缩放因子(1000°/s)
```

### 滤波器参数

```c
/* 滤波器参数 */
#define COMPLEMENTARY_FILTER_ALPHA  0.98f   // 互补滤波器系数
#define KALMAN_Q                    0.001f  // 卡尔曼过程噪声
#define KALMAN_R                    0.1f    // 卡尔曼测量噪声
```

## 扩展功能

### 四元数姿态表示

```c
/* 四元数结构体 */
typedef struct {
    float q0, q1, q2, q3;
} quaternion_t;

/* 四元数姿态更新 */
void Quaternion_Update(imu_data_t *data, quaternion_t *q)
{
    float norm;
    float vx, vy, vz;
    float ex, ey, ez;
    
    // Mahony AHRS算法实现
    // ... 详细实现
}
```

### 零点校准

```c
/* 零点校准 */
void IMU_Calibration(void)
{
    float accel_offset[3] = {0};
    float gyro_offset[3] = {0};
    
    printf("开始IMU校准，请保持设备静止...\\r\\n");
    
    /* 采集1000个样本 */
    for(int i = 0; i < 1000; i++)
    {
        icm20608_raw_data_t raw_data;
        if(ICM20608_Read_Raw_Data(&raw_data) == ICM20608_OK)
        {
            accel_offset[0] += raw_data.accel_x;
            accel_offset[1] += raw_data.accel_y;
            accel_offset[2] += raw_data.accel_z - 4096; // 减去1g
            
            gyro_offset[0] += raw_data.gyro_x;
            gyro_offset[1] += raw_data.gyro_y;
            gyro_offset[2] += raw_data.gyro_z;
        }
        HAL_Delay(5);
    }
    
    /* 计算平均偏移 */
    for(int i = 0; i < 3; i++)
    {
        accel_offset[i] /= 1000.0f;
        gyro_offset[i] /= 1000.0f;
    }
    
    printf("校准完成!\\r\\n");
    printf("加速度偏移: %.2f, %.2f, %.2f\\r\\n", 
           accel_offset[0], accel_offset[1], accel_offset[2]);
    printf("陀螺仪偏移: %.2f, %.2f, %.2f\\r\\n", 
           gyro_offset[0], gyro_offset[1], gyro_offset[2]);
}
```

### 步数统计

```c
/* 步数统计 */
uint32_t Step_Counter(imu_data_t *data)
{
    static uint32_t step_count = 0;
    static float last_accel_magnitude = 0;
    static uint8_t step_state = 0;
    static uint32_t last_step_time = 0;
    
    float magnitude = sqrt(data->accel_x * data->accel_x + 
                          data->accel_y * data->accel_y + 
                          data->accel_z * data->accel_z);
    
    /* 简单的峰值检测算法 */
    if(step_state == 0 && magnitude > 11.0f && 
       (HAL_GetTick() - last_step_time) > 300)
    {
        step_state = 1;
    }
    else if(step_state == 1 && magnitude < 9.0f)
    {
        step_count++;
        step_state = 0;
        last_step_time = HAL_GetTick();
    }
    
    last_accel_magnitude = magnitude;
    return step_count;
}
```

## 故障排除

### 常见问题

1. **读取不到设备ID**
   - 检查I2C接线是否正确
   - 确认上拉电阻(4.7kΩ)
   - 检查设备地址设置

2. **数据跳变或异常**
   - 增加数据滤波
   - 检查电源稳定性
   - 降低I2C速率

3. **姿态角漂移**
   - 进行零点校准
   - 调整滤波器参数
   - 检查传感器安装

### 调试方法

1. 使用逻辑分析仪检查I2C时序
2. 通过串口监控原始数据
3. 使用OLED实时显示状态

## 应用场景

1. **无人机飞控**: 姿态控制和导航
2. **平衡车**: 倾角检测和控制
3. **机器人导航**: 姿态感知和路径规划
4. **智能手表**: 运动监测和步数统计
5. **VR设备**: 头部姿态跟踪

## 学习要点

1. **I2C通信**: 掌握I2C接口的使用和调试
2. **传感器融合**: 理解多传感器数据融合原理
3. **姿态解算**: 学习姿态表示和解算算法
4. **数字滤波**: 掌握各种数字滤波算法
5. **实时处理**: 学习实时数据处理和控制

## 版权信息

版权 (c) 2025 MCU Electronics Technology

MIT License - 详见LICENSE文件
