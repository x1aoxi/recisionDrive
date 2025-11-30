[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="../image/logo.png" width="450" height="100"/>
</div>


## STM32F407VET6 ICM20608六轴传感器

[![HAL](https://img.shields.io/badge/HAL-STM32F4-brightgreen.svg)](/Drivers) [![I2C](https://img.shields.io/badge/interface-I2C-blue.svg)](https://www.i2c-bus.org/) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

ICM20608是一款高性能6轴运动传感器，集成了3轴陀螺仪和3轴加速度计。本项目基于STM32F407VET6微控制器，提供了完整的ICM20608传感器驱动程序，支持数据读取、传感器校准、参数配置等功能。该驱动适配STM32 HAL库，具有良好的兼容性和稳定性，适用于姿态检测、运动监测、平衡控制等应用场景。

### 目录

  - [说明](#说明)
  - [硬件平台](#硬件平台)
  - [传感器特性](#传感器特性)
  - [快速开始](#快速开始)
    - [硬件连接](#硬件连接)
    - [软件集成](#软件集成)
    - [初始化传感器](#初始化传感器)
    - [读取传感器数据](#读取传感器数据)
    - [传感器校准](#传感器校准)
  - [API参考](#api参考)
  - [配置说明](#配置说明)
  - [集成步骤](#集成步骤)
  - [测试功能](#测试功能)
  - [数据转换](#数据转换)
  - [注意事项](#注意事项)
  - [故障排除](#故障排除)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

本项目为STM32F407VET6开发板提供了完整的ICM20608六轴传感器驱动解决方案。

/Components/icm20608目录包含了ICM20608 HAL库适配驱动的源文件。

/Components/MultiTimer目录包含了多定时器组件，用于定时任务管理。

/Components/EasyLogger目录包含了日志系统组件，提供调试信息输出。

/Components/ebtn目录包含了按键处理组件，支持多种按键事件。

/Core目录包含了STM32 HAL库核心代码和主程序。

/Drivers目录包含了STM32F4xx HAL驱动库。

/MDK-ARM目录包含了Keil MDK-ARM工程文件。

### 硬件平台

- **主控芯片**: STM32F407VET6 (ARM Cortex-M4, 168MHz)
- **传感器**: ICM20608 (6轴IMU传感器)
- **通信接口**: I2C1 (PB6-SCL, PB7-SDA)
- **开发环境**: Keil MDK-ARM
- **调试接口**: SWD
- **电源**: 3.3V

### 传感器特性

#### ICM20608规格参数
- **陀螺仪量程**: ±250dps, ±500dps, ±1000dps, ±2000dps
- **加速度计量程**: ±2g, ±4g, ±8g, ±16g
- **数字输出**: 16位ADC
- **工作电压**: 2.4V ~ 3.6V
- **工作温度**: -40°C ~ +85°C
- **封装**: 3x3x0.75mm QFN-24

#### 主要功能
- ✅ 传感器初始化和配置
- ✅ 加速度计数据读取 (±2g/±4g/±8g/±16g)
- ✅ 陀螺仪数据读取 (±250dps/±500dps/±1000dps/±2000dps)
- ✅ 传感器校准功能
- ✅ 设备ID检测
- ✅ 参数配置和读取
- ✅ 错误处理和状态检查
- ✅ 温度补偿
- ✅ 数字低通滤波

#### 技术特点
- 基于STM32 HAL库，兼容性好
- 统一的配置文件管理
- 完整的错误处理机制
- 中文友好的注释和文档
- 模块化设计，易于集成
- 支持中断和轮询两种模式

### 快速开始

#### 硬件连接
```
STM32F407VET6    ICM20608
PB6 (I2C1_SCL) ← → SCL
PB7 (I2C1_SDA) ← → SDA
3.3V           ← → VCC
GND            ← → GND
```

#### 软件集成
在main.c中包含头文件：
```c
#include "icm20608_hal.h"
```

#### 初始化传感器
```c
icm20608_device_t icm20608_dev;

// 在main函数中初始化
if (icm20608_init(&icm20608_dev, &hi2c1) == ICM20608_OK) {
    printf("ICM20608初始化成功!\r\n");
} else {
    printf("ICM20608初始化失败!\r\n");
}
```

#### 读取传感器数据
```c
int16_t accel_x, accel_y, accel_z;
int16_t gyro_x, gyro_y, gyro_z;

// 读取加速度计数据
if (icm20608_get_accel(&icm20608_dev, &accel_x, &accel_y, &accel_z) == ICM20608_OK) {
    printf("加速度: X=%d, Y=%d, Z=%d\r\n", accel_x, accel_y, accel_z);
}

// 读取陀螺仪数据
if (icm20608_get_gyro(&icm20608_dev, &gyro_x, &gyro_y, &gyro_z) == ICM20608_OK) {
    printf("陀螺仪: X=%d, Y=%d, Z=%d\r\n", gyro_x, gyro_y, gyro_z);
}
```

#### 传感器校准
```c
// 校准传感器(建议在传感器静止时进行)
if (icm20608_calib_level(&icm20608_dev, 100) == ICM20608_OK) {
    printf("校准完成!\r\n");
}
```

### API参考

#### 初始化函数
- `icm20608_init()` - 初始化传感器
- `icm20608_deinit()` - 反初始化传感器

#### 数据读取函数
- `icm20608_get_accel()` - 获取加速度计数据
- `icm20608_get_gyro()` - 获取陀螺仪数据
- `icm20608_get_device_id()` - 获取设备ID

#### 配置函数
- `icm20608_set_param()` - 设置传感器参数
- `icm20608_get_param()` - 获取传感器参数
- `icm20608_calib_level()` - 校准传感器

### 配置说明

#### I2C配置
- **时钟频率**: 100kHz (可在i2c.c中修改)
- **地址模式**: 7位地址模式
- **设备地址**: 0x68 (AD0=0)

#### 传感器配置
- **默认陀螺仪量程**: ±250dps
- **默认加速度计量程**: ±2g
- **数字低通滤波器**: 使能
- **采样率**: 1kHz

### 集成步骤

#### 1. 添加源文件到工程
将以下文件添加到Keil工程中：
- `Components/icm20608/icm20608_hal.c`
- `Components/icm20608/icm20608_hal.h`
- `Components/icm20608/icm20608_config.h`

#### 2. 添加头文件路径
在Keil工程设置中添加头文件路径：
```
Components/icm20608
```

#### 3. 修改main.c
参考`Components/icm20608/main_example.c`中的示例代码，在main.c中添加：

```c
/* USER CODE BEGIN Includes */
#include "icm20608_hal.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PTD */
static icm20608_device_t icm20608_dev;
/* USER CODE END PTD */
```

#### 4. 初始化和使用
```c
// 初始化
if (icm20608_init(&icm20608_dev, &hi2c1) == ICM20608_OK) {
    printf("ICM20608初始化成功!\r\n");
}

// 读取数据
int16_t accel_x, accel_y, accel_z;
icm20608_get_accel(&icm20608_dev, &accel_x, &accel_y, &accel_z);
```

### 测试功能

项目提供了完整的测试套件：

#### 运行测试
```c
#include "icm20608_test.h"

// 运行完整测试套件
icm20608_full_test_suite();

// 或单独运行各项测试
icm20608_basic_test();           // 基础功能测试
icm20608_calibration_test();     // 校准测试
icm20608_range_test();           // 量程设置测试
icm20608_continuous_read_test(10); // 连续读取测试
```

#### 测试内容
- ✅ 设备ID验证
- ✅ 基础数据读取
- ✅ 传感器校准
- ✅ 量程配置测试
- ✅ 连续读取稳定性测试

### 数据转换

#### 加速度计数据转换
```c
// 根据当前量程转换为g值
float accel_g = (float)raw_accel / ICM20608_ACCEL_SENSITIVITY_2G;  // ±2g量程
```

#### 陀螺仪数据转换
```c
// 根据当前量程转换为°/s值
float gyro_dps = (float)raw_gyro / ICM20608_GYRO_SENSITIVITY_250DPS; // ±250dps量程
```

### 注意事项

1. **I2C配置**: 确保I2C时钟频率不超过400kHz
2. **电源供电**: ICM20608需要3.3V供电
3. **校准时机**: 建议在传感器静止时进行校准
4. **数据处理**: 原始数据需要根据量程进行换算得到实际物理值
5. **错误处理**: 建议在每次API调用后检查返回值
6. **GPIO配置**: 确保I2C引脚配置正确(PB6-SCL, PB7-SDA)
7. **中断使用**: 如需使用中断功能，需要额外配置GPIO中断

### 工程结构
```
06_v2_ICM20608/
├── Components/               # 组件库
│   ├── icm20608/            # ICM20608 驱动
│   │   ├── inc/             # 头文件目录
│   │   │   └── driver_icm20608.h
│   │   ├── src/             # 源文件目录
│   │   │   └── driver_icm20608.c
│   │   └── interface/       # 接口/适配层（如 HAL 适配代码）
│   ├── MultiTimer/          # 多定时器组件
│   ├── EasyLogger/          # 日志组件
│   └── ebtn/                # 按键组件
├── Core/
│   ├── Inc/                 # 核心头文件
│   └── Src/                 # 核心源文件
│       └── main.c          # 主程序
├── Drivers/                 # STM32 HAL驱动
├── MDK-ARM/                 # Keil工程文件
├── Icm20608.ioc            # STM32CubeMX配置文件
└── README.md               # 本文档
```

### 高级功能

#### 中断模式配置
```c
/* 配置数据就绪中断 */
icm20608_result_t icm20608_enable_data_ready_interrupt(icm20608_device_t *dev) {
    uint8_t int_enable = 0x01;  // 使能数据就绪中断
    return icm20608_write_reg(dev, ICM20608_REG_INT_ENABLE, int_enable);
}

/* 中断服务函数 */
void EXTI0_IRQHandler(void) {
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);

        /* 读取传感器数据 */
        icm20608_data_ready_callback();
    }
}
```

#### FIFO模式
```c
/* 使能FIFO */
icm20608_result_t icm20608_enable_fifo(icm20608_device_t *dev) {
    uint8_t fifo_en = 0x78;  // 使能加速度计和陀螺仪FIFO
    return icm20608_write_reg(dev, ICM20608_REG_FIFO_EN, fifo_en);
}

/* 读取FIFO数据 */
icm20608_result_t icm20608_read_fifo(icm20608_device_t *dev, uint8_t *data, uint16_t len) {
    return icm20608_read_regs(dev, ICM20608_REG_FIFO_R_W, data, len);
}
```

#### 温度读取
```c
/* 读取温度数据 */
icm20608_result_t icm20608_get_temperature(icm20608_device_t *dev, float *temp) {
    int16_t temp_raw;
    icm20608_result_t result;

    result = icm20608_read_reg16(dev, ICM20608_REG_TEMP_OUT_H, &temp_raw);
    if (result == ICM20608_OK) {
        *temp = (float)temp_raw / 326.8f + 25.0f;  // 温度转换公式
    }

    return result;
}
```

### 应用场景
- 无人机姿态控制
- 机器人平衡系统
- 游戏手柄和VR设备
- 运动监测设备
- 车辆稳定性控制
- 地震监测仪器

### 性能优化建议

1. **读取频率**: 建议读取频率不超过1kHz
2. **数据滤波**: 可在应用层添加低通滤波器
3. **批量读取**: 可一次性读取多个寄存器减少I2C通信次数
4. **中断模式**: 对于高频应用建议使用数据就绪中断

### 故障排除

#### 常见问题
1. **初始化失败**: 检查I2C连接和电源
2. **设备ID错误**: 确认ICM20608型号和I2C地址
3. **数据读取失败**: 检查I2C时序和电源稳定性
4. **校准异常**: 确保传感器在校准时保持静止

#### 调试方法
1. 使用示波器检查I2C信号
2. 启用调试输出查看详细信息
3. 运行测试套件验证功能
4. 检查返回值确定错误类型

#### 错误代码说明
```c
typedef enum {
    ICM20608_OK = 0,           // 操作成功
    ICM20608_ERROR = 1,        // 一般错误
    ICM20608_TIMEOUT = 2,      // 超时错误
    ICM20608_INVALID_PARAM = 3, // 参数错误
    ICM20608_NOT_READY = 4,    // 设备未就绪
    ICM20608_CALIB_ERROR = 5   // 校准错误
} icm20608_result_t;
```

### 自定义配置

#### 修改I2C接口
```c
extern I2C_HandleTypeDef hi2c2;  // 使用I2C2接口
#define ICM20608_I2C_HANDLE &hi2c2
```

#### 修改I2C地址
```c
#define ICM20608_I2C_ADDRESS    0x69  // AD0=1时的地址
```

#### 配置量程
```c
/* 设置陀螺仪量程 */
icm20608_set_gyro_range(&icm20608_dev, ICM20608_GYRO_RANGE_500DPS);

/* 设置加速度计量程 */
icm20608_set_accel_range(&icm20608_dev, ICM20608_ACCEL_RANGE_4G);
```

#### 配置滤波器
```c
/* 设置数字低通滤波器 */
icm20608_set_dlpf(&icm20608_dev, ICM20608_DLPF_BW_20HZ);

/* 设置采样率 */
icm20608_set_sample_rate(&icm20608_dev, 100);  // 100Hz采样率
```

### 完整使用示例

#### example basic

```c
#include "icm20608_hal.h"

icm20608_device_t icm20608_dev;

int main(void)
{
    /* 系统初始化 */
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();

    /* 初始化传感器 */
    if (icm20608_init(&icm20608_dev, &hi2c1) != ICM20608_OK) {
        printf("ICM20608初始化失败\r\n");
        return -1;
    }

    printf("ICM20608初始化成功\r\n");

    /* 配置传感器 */
    icm20608_set_gyro_range(&icm20608_dev, ICM20608_GYRO_RANGE_250DPS);
    icm20608_set_accel_range(&icm20608_dev, ICM20608_ACCEL_RANGE_2G);

    /* 校准传感器 */
    printf("开始校准，请保持传感器静止...\r\n");
    if (icm20608_calib_level(&icm20608_dev, 100) == ICM20608_OK) {
        printf("校准完成\r\n");
    }

    while (1)
    {
        int16_t accel[3], gyro[3];
        float temp;

        /* 读取加速度计数据 */
        if (icm20608_get_accel(&icm20608_dev, &accel[0], &accel[1], &accel[2]) == ICM20608_OK) {
            printf("加速度: X=%d, Y=%d, Z=%d\r\n", accel[0], accel[1], accel[2]);
        }

        /* 读取陀螺仪数据 */
        if (icm20608_get_gyro(&icm20608_dev, &gyro[0], &gyro[1], &gyro[2]) == ICM20608_OK) {
            printf("陀螺仪: X=%d, Y=%d, Z=%d\r\n", gyro[0], gyro[1], gyro[2]);
        }

        /* 读取温度 */
        if (icm20608_get_temperature(&icm20608_dev, &temp) == ICM20608_OK) {
            printf("温度: %.2f°C\r\n", temp);
        }

        HAL_Delay(100);
    }
}
```

#### example interrupt

```c
#include "icm20608_hal.h"

icm20608_device_t icm20608_dev;
volatile uint8_t data_ready = 0;

/* 中断回调函数 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_0) {  // 假设使用PA0作为中断引脚
        data_ready = 1;
    }
}

int main(void)
{
    /* 系统初始化 */
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();

    /* 初始化传感器 */
    if (icm20608_init(&icm20608_dev, &hi2c1) != ICM20608_OK) {
        printf("ICM20608初始化失败\r\n");
        return -1;
    }

    /* 使能数据就绪中断 */
    icm20608_enable_data_ready_interrupt(&icm20608_dev);

    printf("ICM20608中断模式启动\r\n");

    while (1)
    {
        if (data_ready) {
            data_ready = 0;

            int16_t accel[3], gyro[3];

            /* 读取传感器数据 */
            icm20608_get_accel(&icm20608_dev, &accel[0], &accel[1], &accel[2]);
            icm20608_get_gyro(&icm20608_dev, &gyro[0], &gyro[1], &gyro[2]);

            printf("A: %d,%d,%d G: %d,%d,%d\r\n",
                   accel[0], accel[1], accel[2],
                   gyro[0], gyro[1], gyro[2]);
        }

        HAL_Delay(1);
    }
}
```

### 数据处理算法

#### 姿态解算示例
```c
typedef struct {
    float roll;   // 横滚角
    float pitch;  // 俯仰角
    float yaw;    // 偏航角
} attitude_t;

/* 简单的互补滤波器 */
void attitude_update(attitude_t *att, float ax, float ay, float az,
                    float gx, float gy, float gz, float dt)
{
    float alpha = 0.98f;  // 互补滤波器系数

    /* 加速度计计算角度 */
    float accel_roll = atan2f(ay, az) * 180.0f / M_PI;
    float accel_pitch = atan2f(-ax, sqrtf(ay*ay + az*az)) * 180.0f / M_PI;

    /* 陀螺仪积分 */
    att->roll = alpha * (att->roll + gx * dt) + (1.0f - alpha) * accel_roll;
    att->pitch = alpha * (att->pitch + gy * dt) + (1.0f - alpha) * accel_pitch;
    att->yaw += gz * dt;
}
```

### 文档

**在线文档**:
- [ICM20608数据手册](https://invensense.tdk.com/wp-content/uploads/2016/06/DS-000179-ICM-20608-G-v1.0.pdf)
- [STM32 HAL库文档](https://www.st.com/resource/en/user_manual/dm00105879.pdf)

**离线文档**: /doc目录包含相关技术文档

### 版本历史
- v1.0.0 - 初始版本，完成基本功能适配
- v1.0.1 - 添加完整测试套件和示例代码
- v1.0.2 - 优化I2C配置和错误处理
- v1.1.0 - 添加中断模式和FIFO支持
- v1.1.1 - 增加姿态解算示例和性能优化

### 贡献

请参考CONTRIBUTING.md提交代码贡献。

### 版权

版权 (c) 2025 MCU Electronics Technology 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称"软件"）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是"如此"提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

如有技术问题或建议，请联系技术支持获取更多帮助和资源。
