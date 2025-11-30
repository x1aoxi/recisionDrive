# ICM20608 六轴姿态传感器欧拉角解算

## 功能概述

本项目实现了基于ICM20608六轴IMU传感器的欧拉角解算功能，使用互补滤波器算法融合陀螺仪和加速度计数据，输出Roll、Pitch、Yaw三个欧拉角。

## 核心特性

### 1. 数据处理
- **传感器支持**: ICM20608、ICM20689、MPU6050等
- **数据单位**: 陀螺仪(°/s)、加速度计(g)
- **输出单位**: 欧拉角(度)
- **更新频率**: 100Hz (10ms周期)

### 2. 自动校准
- **零偏校准**: 启动时自动进行2秒静态校准
- **校准阈值**: 静态检测≤5°/s
- **校准指示**: 串口输出校准进度和结果
- **动态调整**: 校准完成后自动调整滤波器参数

### 3. 算法特性
- **互补滤波器**: 融合陀螺仪和加速度计数据
- **四元数计算**: 内部使用四元数避免万向锁
- **数据保护**: 异常值检测和处理
- **漂移检测**: 定期监测并提示重新校准

## 使用方法

### 1. 初始化
```c
void Gyroscope_Init(void)
{
  // 初始化ICM20608硬件
  ICM206xx_Init();
  
  // 初始化陀螺仪驱动
  Gyroscope_Driver_Init();
  
  // 开始校准
  Gyroscope_Calibrate_Start();
}
```

### 2. 主循环更新
```c
void Gyroscope_Task(void)
{
  uint32_t current_time = HAL_GetTick();
  float dt = (current_time - last_gyro_time) / 1000.0f;
  
  // 读取原始数据
  ICM206xx_Read_Data(&icm20608.gyro, &icm20608.accel, &icm20608.temperature);
  
  // 更新欧拉角解算
  Gyroscope_Update_Euler(&icm20608.gyro, &icm20608.accel, dt);
  
  // 获取解算后的欧拉角
  Gyroscope_Get_Euler_Angles(&icm20608.Roll, &icm20608.Pitch, &icm20608.Yaw);
  
  last_gyro_time = current_time;
}
```

### 3. 获取数据
```c
float roll, pitch, yaw;
Gyroscope_Get_Euler_Angles(&roll, &pitch, &yaw);

printf("欧拉角: Roll=%.2f°, Pitch=%.2f°, Yaw=%.2f°\n", roll, pitch, yaw);
```

## API 接口

### 核心函数
- `Gyroscope_Driver_Init()` - 初始化驱动
- `Gyroscope_Update_Euler()` - 更新欧拉角解算
- `Gyroscope_Get_Euler_Angles()` - 获取欧拉角

### 校准函数
- `Gyroscope_Calibrate_Start()` - 开始校准
- `Gyroscope_Reset_Attitude()` - 重置姿态

### 状态函数
- `Gyroscope_Print_Status()` - 打印状态信息
- `Gyroscope_Get_Yaw_Rate()` - 获取偏航角速率

## 数据结构

### EulerAngles_t
```c
typedef struct {
    float roll;     // 横滚角 (度)
    float pitch;    // 俯仰角 (度) 
    float yaw;      // 偏航角 (度)
    float q0, q1, q2, q3;  // 四元数
    float gx_bias, gy_bias, gz_bias;  // 陀螺仪零偏
    uint8_t calibrated;  // 校准状态标志
    uint32_t last_update_time;  // 上次更新时间
} EulerAngles_t;
```

## 调试输出

### 校准过程
```
Gyroscope Calibration Started...
Calibration Progress: 50.0% (100 samples)
Calibration Complete! Bias: X=0.123, Y=-0.456, Z=0.789
```

### 状态信息
```
=== Gyroscope Status ===
Calibrated: Yes
Euler Angles: Roll=1.23, Pitch=-2.45, Yaw=156.78
Quaternion: q0=0.987, q1=0.012, q2=-0.023, q3=0.156
Gyro Bias: X=0.123, Y=-0.456, Z=0.789
Filter Alpha: 0.020
Last Update: 10 ms ago
========================
```

## 注意事项

### 1. 校准要求
- 校准期间保持传感器静止
- 避免振动和移动
- 确保校准环境稳定

### 2. 使用建议
- 定期检查校准状态
- 监控漂移提示信息
- 根据应用需求调整滤波参数

### 3. 性能优化
- 合理设置更新频率
- 避免频繁的状态打印
- 根据精度要求调整算法参数

## 参数配置

### 滤波器参数
- 初始alpha: 0.85 (校准阶段)
- 校准后alpha: 0.02 (正常运行)

### 阈值设置
- 静态检测: 5.0°/s
- 漂移检测: 10.0°/s
- 校准时间: 2000ms

### 数据范围
- 陀螺仪: ±2000°/s
- 加速度计: ±16g

## 文件结构

```
User/
├── Driver/
│   ├── gyroscope_driver.h  # 驱动接口定义
│   └── gyroscope_driver.c  # 欧拉角解算实现
├── App/
│   ├── gyroscope_app.h     # 应用层接口
│   └── gyroscope_app.c     # 应用层实现
└── Module/
    └── ICM20608/
        ├── icm20608.h      # 传感器驱动
        └── icm20608.c      # 硬件接口
```

## 更新历史

- v1.0.0 - 初始版本，实现基本欧拉角解算
- 支持多种IMU传感器
- 自动校准功能
- 互补滤波器算法
- 完整的调试输出