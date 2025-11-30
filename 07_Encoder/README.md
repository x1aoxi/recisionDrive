# PrecisionDrive

> 精准电机控制系统 | Precision Motor Control System

基于 STM32F407VET6 的高精度电机控制系统，支持多种运动模式和精确圈数控制。

## 功能概述

| 模式 | 功能 | 参数 |
|------|------|------|
| 基础运行 | 固定转速正反转 | 30rpm |
| 三档变速 | 三档切换 | 30/50/80 rpm |
| 加速度测试 | 匀加速运动 | 5或20 rpm/s |
| 梯形曲线 | 加速→匀速→减速 | 可配置时间 |
| 精准圈数 | 指定圈数停止 | 1-20圈 |

## 功能详解

### 1. 基础运行模式 (Basic Run)

固定转速运行，支持正反转切换。

- **正转**: 30rpm (PWM=568)
- **反转**: 30rpm (PWM=568)
- **操作**: KEY1/KEY2 切换方向，KEY3 启动，KEY4 停止

### 2. 三档变速模式 (Speed Gear)

三档位速度切换，运行中可实时调整。

| 档位 | 转速 | PWM值 |
|------|------|-------|
| 低速档 | 30 rpm | 568 |
| 中速档 | 50 rpm | 595 |
| 高速档 | 80 rpm | 634 |

- **操作**: KEY1 升档，KEY2 降档，KEY3 启动，KEY4 停止

### 3. 加速度测试模式 (Acceleration)

从起始转速开始匀加速，测试电机加速性能。

| 参数 | 值 |
|------|------|
| 起始转速 | 16 rpm |
| 最大转速 | 200 rpm |
| 低加速度 | 5 rpm/s |
| 高加速度 | 20 rpm/s |

- **操作**: KEY1/KEY2 切换加速度档位，KEY3 启动，KEY4 停止
- **公式**: `target_rpm += accel_rate × 0.01` (每10ms更新)

### 4. 梯形曲线模式 (Trapezoid)

完整的加速→恒速→减速曲线，自动完成后停止。

```
转速
  ↑
max ├────────────────┐
    │   /            \
    │  /              \
    │ /                \
min ├─                  ──→ 时间
    │← 5s →│←  10s  →│← 5s →│
      加速      恒速      减速
```

| 参数 | 值 |
|------|------|
| 起始转速 | 15 rpm |
| 最大转速 | 115 rpm |
| 加速度 | 20 rpm/s |
| 减速度 | 20 rpm/s |
| 加速时间 | 5000 ms |
| 恒速时间 | 10000 ms |
| 减速时间 | 5000 ms |

- **操作**: KEY3 启动，KEY4 停止，完成后自动停止
- **状态机**: IDLE → ACCEL → CONST → DECEL → FINISHED

### 5. 精准圈数模式 (Circle Control)

指定转动圈数后自动停止，基于编码器脉冲计数。

| 参数 | 值 |
|------|------|
| 运行转速 | 16 rpm |
| 编码器PPR | 1551 |
| 圈数范围 | 1-20 圈 |

- **操作**: KEY1 增加圈数，KEY2 减少圈数，KEY3 启动，KEY4 停止
- **计算**: `current_circles = delta_count / PPR`
- **完成条件**: `delta_count >= target_circles × PPR`

## 硬件配置

### MCU
- STM32F407VET6 @ 168MHz

### 电机驱动
- 驱动芯片: DRV8871DDA
- PWM: TIM1_CH3/CH4 (右电机)
- 线性关系: `PWM = 1.30 × RPM + 529.2`

### 编码器
- 右编码器: TIM4 (PA6/PA7)
- PPR: 1551 (实测)
- 采样周期: 10ms

### OLED显示
- 型号: 0.91寸 SSD1306
- 分辨率: 128×32
- 接口: I2C (PB10/PB11)

### 按键
| 按键 | 引脚 | 功能 |
|------|------|------|
| KEY1 | PC0 | 上/切换 |
| KEY2 | PC1 | 下/切换 |
| KEY3 | PC2 | 确认/启动 |
| KEY4 | PC3 | 返回/停止 |

## 工程结构

```
07_Encoder/
├── Core/                    # HAL初始化代码
├── User/
│   ├── App/                 # 应用层
│   │   ├── motor_app.c      # 电机控制(核心)
│   │   ├── encoder_app.c    # 编码器采集
│   │   ├── ui_menu_app.c    # 菜单系统
│   │   ├── ui_page_app.c    # 页面绘制
│   │   └── ...
│   ├── Driver/              # 驱动层
│   │   ├── motor_driver.c   # 电机PWM
│   │   ├── encoder_driver.c # 编码器定时器
│   │   └── ...
│   ├── Module/              # 外设模块
│   │   ├── PID/             # PID算法
│   │   ├── Ebtn/            # 按键库
│   │   └── 0.91 OLED/       # OLED底层
│   ├── Scheduler.c          # 任务调度器
│   └── Scheduler_Task.c     # 系统初始化
└── MDK-ARM/                 # Keil工程
```

## 电机数量配置

### 切换单/双电机模式

在 [motor_app.h:13](User/App/motor_app.h#L13) 修改 `MOTOR_COUNT` 配置：

```c
#define MOTOR_COUNT 1    // 1=仅右电机 (当前) | 2=双电机
```

- **MOTOR_COUNT = 1** (当前):
  - 仅使用右电机 (TIM1)
  - 仅使用右编码器 (TIM4)
  - 右电机代码完全独立，不受影响

- **MOTOR_COUNT = 2** (可选):
  - 启用左电机 (TIM2) 和右电机 (TIM1)
  - 启用左编码器 (TIM3) 和右编码器 (TIM4)
  - 两个电机同步控制，速度取平均值
  - 需要左电机硬件连接

## 参数配置

所有运行参数集中在 `motor_app.c` 的 `pwm_config` 结构体中：

```c
static const MotorPWMConfig pwm_config = {
    // 基本运行 (30rpm)
    .basic_run_pwm = 568,
    .basic_run_reverse_pwm = 568,

    // 三档变速
    .gear_low_pwm = 568,   // 30rpm
    .gear_mid_pwm = 595,   // 50rpm
    .gear_high_pwm = 634,  // 80rpm

    // 加速度测试
    .accel_start_rpm = 16.0f,
    .accel_max_rpm = 200.0f,
    .accel_rate_low = 5.0f,    // 5 rpm/s
    .accel_rate_high = 20.0f,  // 20 rpm/s

    // 梯形曲线
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
```

## 使用方法

1. 打开 `MDK-ARM/07_Encoder.uvprojx`
2. 编译并烧录
3. 使用按键操作菜单

### 串口调试
- 波特率: 115200
- 格式: 8N1

## API接口

```c
// 模式控制
void MotorApp_SetMode(MotorMode mode);
void MotorApp_Start(void);
void MotorApp_Stop(void);

// 状态查询
float MotorApp_GetCurrentRPM(void);
MotorState* MotorApp_GetState(void);
uint8_t MotorApp_IsRunning(void);

// 档位控制
void MotorApp_SpeedGear_SetGear(SpeedGear gear);

// 圈数控制
void MotorApp_CircleControl_SetTarget(uint8_t circles);
```

## 任务调度

| 任务 | 周期 | 位置 |
|------|------|------|
| Encoder_Task | 10ms | TIM2中断 |
| Motor_Task | 10ms | TIM2中断 |
| PID_Task | 10ms | TIM2中断 |
| Led_Task | 1ms | 主循环 |
| Key_Task | 10ms | 主循环 |
| Oled_Task | 10ms | 主循环 |

## 版本

- v2.3 (2025-11-30) - 仓库清理版
  - 添加 .gitignore 排除编译产物
  - 移除 LVGL 库和冗余工程目录
  - 仓库体积大幅减小

- v2.2 (2025-11-29) - 双电机支持版
  - 添加条件编译支持单/双电机模式
  - 左电机完全独立可选
  - 不影响现有右电机功能

- v2.1 (2025-11-29) - 代码精简版
  - 移除冗余注释和未使用变量
  - 合并重复计时器
  - 统一参数配置

---

MIT License
