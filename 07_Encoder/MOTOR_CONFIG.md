# 电机配置快速参考

## 一键切换：单电机 ↔ 双电机

### 当前模式：单电机（仅右电机）

```c
// motor_app.h 第13行
#define MOTOR_COUNT 1  // 仅右电机
```

### 切换到双电机模式

只需修改一行代码：

```c
// motor_app.h 第13行
#define MOTOR_COUNT 2  // 启用左右双电机
```

然后重新编译即可！

---

## 工作原理

### MOTOR_COUNT = 1 (单电机模式)

受影响的代码部分会被编译器**完全忽略**：

| 文件 | 未编译部分 |
|------|-----------|
| motor_app.c | `#if MOTOR_COUNT == 2` 内的左电机代码 |
| motor_app.h | left_rpm 字段 |
| encoder_app.c | 左编码器初始化和更新 |
| pid_app.c | 左电机PID控制 |

**结果**：右电机代码 100% 独立运行

### MOTOR_COUNT = 2 (双电机模式)

所有左电机代码被启用：

- 左电机 (TIM2) 和右电机 (TIM1) 同时工作
- 左编码器 (TIM3) 和右编码器 (TIM4) 同时采样
- 双电机速度取平均值

---

## 硬件对应关系

### 单电机模式 (MOTOR_COUNT = 1)

| 器件 | TIM | 通道 |
|------|-----|------|
| 右电机 | TIM1 | CH3/CH4 |
| 右编码器 | TIM4 | CH1/CH2 (PA6/PA7) |

### 双电机模式 (MOTOR_COUNT = 2)

| 器件 | TIM | 通道 |
|------|-----|------|
| 左电机 | TIM2 | CH3/CH4 |
| 右电机 | TIM1 | CH3/CH4 |
| 左编码器 | TIM3 | CH1/CH2 (PA0/PA1) |
| 右编码器 | TIM4 | CH1/CH2 (PA6/PA7) |

---

## 修改其他参数

### PWM曲线校准

所有参数在 **motor_app.c:80-108** 的 `pwm_config` 结构体：

```c
static const MotorPWMConfig pwm_config = {
    // 基本运行 (30rpm)
    .basic_run_pwm = 568,
    .basic_run_reverse_pwm = 568,

    // 三档变速
    .gear_low_pwm = 568,   // 30rpm
    .gear_mid_pwm = 595,   // 50rpm
    .gear_high_pwm = 634,  // 80rpm

    // ... 其他参数
};
```

**修改后无需改其他地方**，所有模式自动使用新参数。

### PID参数调整

在 **pid_app.c** 中修改：

```c
PidParams_t pid_params_right = {
    .kp = 33.0f,  // 比例系数
    .ki = 4.5f,   // 积分系数
    .kd = 0.0f,   // 微分系数
    .out_min = -999.0f,
    .out_max = 999.0f,
};
```

双电机模式下也有 `pid_params_left` 对应左电机。

---

## 故障排除

### 编译错误：Undefined symbol left_motor

**原因**：引用了 left_motor 但 MOTOR_COUNT = 1

**解决**：设置 `MOTOR_COUNT = 2` 或删除 left_motor 引用

### 编译错误：Undefined symbol left_encoder

**原因**：同上

**解决**：设置 `MOTOR_COUNT = 2`

### 左电机不转

**检查清单**：
1. 确认 `MOTOR_COUNT = 2`
2. 检查 TIM2 PWM输出（示波器）
3. 确认左电机硬件连接
4. 检查 DRV8871 驱动芯片

---

## 完整编译流程

```
1. 修改 motor_app.h 第13行
   #define MOTOR_COUNT 2

2. 清除编译
   Project → Rebuild All (F7)

3. 烧录
   Download (F8)

4. 完成！
```

---

## 代码原理

所有条件编译使用 `#if MOTOR_COUNT == 2` 结构：

```c
#if MOTOR_COUNT == 2
    // 这里只在 MOTOR_COUNT = 2 时编译
    Motor_Set_Speed(&left_motor, pwm_value);
#endif
    // 这里总是编译
    Motor_Set_Speed(&right_motor, pwm_value);
```

优点：
- ✅ 无运行时开销（编译期决定）
- ✅ 代码清晰易维护
- ✅ 容易快速切换
- ✅ 右电机性能不受影响

---

最后更新：2025-11-29
