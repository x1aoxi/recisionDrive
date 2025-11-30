[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="./image/logo.png" width="450" height="100"/>
</div>


## STM32F407VET6 MainBoard V2.0 示例工程集

[![HAL](https://img.shields.io/badge/HAL-STM32F4-brightgreen.svg)](/Drivers) [![Keil](https://img.shields.io/badge/IDE-Keil_MDK-blue.svg)](https://www.keil.com/) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

STM32F407VET6 MainBoard V2.0是一款基于ARM Cortex-M4内核的高性能微控制器开发板，运行频率高达168MHz。本项目包含7个完整的示例工程，涵盖了嵌入式开发中的常用外设和功能模块，包括RGB LED控制、按键输入、串口通信、SD卡文件系统、SPI Flash存储、六轴传感器和温湿度传感器等应用。所有工程均基于STM32 HAL库开发，提供了完整的硬件驱动、中间件集成和应用示例，适用于各种传感、控制和数据采集应用场景。

### 目录

  - [说明](#说明)
  - [硬件平台](#硬件平台)
  - [工程列表](#工程列表)
    - [00_v2_RGB_Blink](#00_v2_rgb_blink)
    - [01_v2_BTN_Input](#01_v2_btn_input)
    - [03_v2_USART](#03_v2_usart)
    - [04_v2_SDIO](#04_v2_sdio)
    - [05_v2_W25QXX](#05_v2_w25qxx)
    - [06_v2_ICM20608](#06_v2_icm20608)
    - [07_v2_AHT20](#07_v2_aht20)
  - [通用组件](#通用组件)
  - [快速开始](#快速开始)
  - [工程结构](#工程结构)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

本项目为STM32F407VET6 MainBoard V2.0开发板提供了7个完整的示例工程，每个工程都专注于特定的功能模块和外设应用。

/00_v2_RGB_Blink目录包含RGB LED闪烁控制示例。

/01_v2_BTN_Input目录包含多功能按键输入处理示例。

/03_v2_USART目录包含串口通信和日志输出示例。

/04_v2_SDIO目录包含SD卡文件系统操作示例。

/05_v2_W25QXX目录包含SPI Flash存储器读写示例。

/06_v2_ICM20608目录包含六轴IMU传感器数据采集示例。

/07_v2_AHT20目录包含温湿度传感器测量示例。

每个工程的/Components目录包含了相关的中间件和驱动组件。

每个工程的/Core目录包含了STM32 HAL库核心代码。

每个工程的/Drivers目录包含了STM32F4xx HAL驱动库。

每个工程的/MDK-ARM目录包含了Keil MDK-ARM工程文件。

### 硬件平台

- **主控芯片**: STM32F407VET6 (ARM Cortex-M4, 168MHz)
- **Flash**: 512KB
- **SRAM**: 192KB
- **开发板**: Mcore MainBoard V2.0
- **开发环境**: Keil MDK-ARM
- **调试接口**: SWD
- **电源**: 3.3V/5V

### 工程列表

#### 00_v2_RGB_Blink

RGB LED闪烁控制示例，实现三色LED的循环闪烁效果。

**主要特性**:
- 红、绿、蓝三色LED循环切换
- 可配置切换间隔时间(默认200ms)
- 基于HAL_GetTick()的非阻塞延时

**硬件连接**:
- PC0 - 红色LED (低电平点亮)
- PC1 - 绿色LED (低电平点亮)  
- PC2 - 蓝色LED (低电平点亮)

#### 01_v2_BTN_Input

多功能按键输入处理示例，支持单击、双击、长按检测。

**主要特性**:
- 支持单击、双击、长按检测
- 硬件消抖和软件消抖
- 基于ebtn组件的事件驱动机制
- MultiTimer软件定时器管理

**硬件连接**:
- PC3 - 用户按键 (低电平有效)

**按键功能**:
- 单击: 切换红色LED状态
- 双击: 切换绿色LED状态
- 长按: 切换蓝色LED状态

#### 03_v2_USART

串口通信和日志输出示例，实现UART通信和调试信息输出。

**主要特性**:
- USART1串口通信 (115200bps)
- printf重定向到UART
- EasyLogger日志系统集成
- 按键事件日志输出

**硬件连接**:
- PA9 - USART1_TX
- PA10 - USART1_RX

#### 04_v2_SDIO

SD卡文件系统操作示例，实现文件的创建、读取和写入。

**主要特性**:
- SDIO接口SD卡驱动
- FatFs文件系统支持
- 文件创建、读取、写入操作
- SD卡信息读取和显示

**硬件连接**:
- PC8~PC12, PD2 - SDIO接口

#### 05_v2_W25QXX

SPI Flash存储器读写示例，实现外部Flash的数据存储。

**主要特性**:
- SPI接口Flash驱动
- 扇区擦除、页编程
- 数据读取和验证
- 设备ID检测

**硬件连接**:
- PA4~PA7 - SPI1接口

#### 06_v2_ICM20608

六轴IMU传感器数据采集示例，获取加速度和角速度数据。

**主要特性**:
- I2C接口传感器驱动
- 三轴加速度计数据读取
- 三轴陀螺仪数据读取
- 传感器校准功能

**硬件连接**:
- PB6 - I2C1_SCL
- PB7 - I2C1_SDA

#### 07_v2_AHT20

温湿度传感器测量示例，实现环境温湿度的精确测量。

**主要特性**:
- I2C接口传感器驱动
- 温度和湿度数据读取
- 高精度测量
- 自动校准功能

**硬件连接**:
- PB6 - I2C1_SCL
- PB7 - I2C1_SDA

### 通用组件

**MultiTimer**: 软件定时器组件，支持多个定时器任务和非阻塞定时机制。

**EasyLogger**: 日志系统组件，提供多级别日志输出和彩色显示。

**ebtn**: 按键处理组件，支持多种按键事件检测和硬件消抖。

### 快速开始

参考每个工程目录下的具体说明，完成硬件连接和软件配置。

#### 环境准备

```bash
# 安装Keil MDK-ARM开发环境
# 准备STM32F407VET6开发板
# 连接ST-Link调试器
```

#### 工程编译

```bash
# 进入对应工程目录
cd 00_v2_RGB_Blink

# 使用Keil打开工程文件
# 双击 MDK-ARM/*.uvprojx 文件

# 编译工程
# 按F7或点击编译按钮
```

#### 程序下载

```bash
# 连接ST-Link到开发板
# 在Keil中配置调试器
# 按F8下载程序到Flash
```

## 使用示例

### example RGB_Blink

```c
#include "main.h"
#include "gpio.h"

/* RGB LED闪烁示例 */
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    while (1)
    {
        RGB_Update();  // 更新RGB状态
        HAL_Delay(1);  // 短暂延时
    }
}
```

### example BTN_Input

```c
#include "main.h"
#include "ebtn.h"
#include "multiTimer.h"

/* 按键输入处理示例 */
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    multiTimerInstall(bsp_timer_tick);
    app_btn_init();
    multiTimerStart(&mt_key, MT_KEY_TIME, mt_cb_key, NULL);

    while (1)
    {
        multiTimerYield();  // 处理定时器任务
    }
}
```

### example USART

```c
#include "main.h"
#include "usart.h"
#include "elog.h"

/* 串口通信示例 */
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    elog_init();
    elog_start();

    elog_i("main", "系统启动成功");

    while (1)
    {
        elog_i("main", "系统运行中...");
        HAL_Delay(1000);
    }
}
```

### example SDIO

```c
#include "main.h"
#include "sdio.h"
#include "fatfs.h"

/* SD卡文件系统示例 */
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SDIO_SD_Init();
    MX_FATFS_Init();

    FATFS fs;
    FIL file;
    FRESULT f_res;

    /* 挂载文件系统 */
    f_res = f_mount(&fs, "0:", 1);
    if (f_res == FR_OK) {
        printf("SD卡挂载成功\r\n");

        /* 创建并写入文件 */
        f_res = f_open(&file, "0:/test.txt", FA_CREATE_ALWAYS | FA_WRITE);
        if (f_res == FR_OK) {
            f_write(&file, "Hello SD Card!", 14, NULL);
            f_close(&file);
            printf("文件写入成功\r\n");
        }
    }

    while (1)
    {
        HAL_Delay(1000);
    }
}
```

### example W25QXX

```c
#include "main.h"
#include "spi.h"
#include "gd25qxx.h"

/* SPI Flash存储示例 */
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();

    /* 初始化Flash */
    if (gd25qxx_init() == 0) {
        printf("W25QXX初始化成功\r\n");

        uint8_t write_data[] = "Hello Flash!";
        uint8_t read_data[32];

        /* 擦除扇区 */
        gd25qxx_sector_erase(0x000000);

        /* 写入数据 */
        gd25qxx_page_program(0x000000, write_data, sizeof(write_data));

        /* 读取数据 */
        gd25qxx_read_data(0x000000, read_data, sizeof(read_data));

        printf("读取数据: %s\r\n", read_data);
    }

    while (1)
    {
        HAL_Delay(1000);
    }
}
```

### example ICM20608

```c
#include "main.h"
#include "i2c.h"
#include "icm20608_hal.h"

/* 六轴传感器示例 */
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();

    icm20608_device_t icm20608_dev;

    /* 初始化传感器 */
    if (icm20608_init(&icm20608_dev, &hi2c1) == ICM20608_OK) {
        printf("ICM20608初始化成功\r\n");

        while (1)
        {
            int16_t accel_x, accel_y, accel_z;
            int16_t gyro_x, gyro_y, gyro_z;

            /* 读取加速度计数据 */
            if (icm20608_get_accel(&icm20608_dev, &accel_x, &accel_y, &accel_z) == ICM20608_OK) {
                printf("加速度: X=%d, Y=%d, Z=%d\r\n", accel_x, accel_y, accel_z);
            }

            /* 读取陀螺仪数据 */
            if (icm20608_get_gyro(&icm20608_dev, &gyro_x, &gyro_y, &gyro_z) == ICM20608_OK) {
                printf("陀螺仪: X=%d, Y=%d, Z=%d\r\n", gyro_x, gyro_y, gyro_z);
            }

            HAL_Delay(100);
        }
    }

    return 0;
}
```

### example AHT20

```c
#include "main.h"
#include "i2c.h"
#include "aht20.h"

/* 温湿度传感器示例 */
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();

    /* 初始化传感器 */
    if (aht20_init(&hi2c1) == AHT20_OK) {
        printf("AHT20初始化成功\r\n");

        while (1)
        {
            float temperature, humidity;

            /* 一次性测量 */
            if (aht20_measure_once(&temperature, &humidity) == AHT20_OK) {
                printf("温度: %.2f°C\r\n", temperature);
                printf("湿度: %.2f%%RH\r\n", humidity);
            }

            HAL_Delay(2000);  // 2秒测量一次
        }
    }

    return 0;
}
```

### 工程结构

```
MainBoard_V2.0/
├── 00_v2_RGB_Blink/          # RGB LED闪烁控制
│   ├── Core/                 # 核心代码
│   ├── Drivers/              # HAL驱动
│   ├── MDK-ARM/              # Keil工程
│   └── README.md             # 工程说明
├── 01_v2_BTN_Input/          # 多功能按键输入
│   ├── Components/           # 按键和定时器组件
│   ├── Core/                 # 核心代码
│   ├── Drivers/              # HAL驱动
│   ├── MDK-ARM/              # Keil工程
│   └── README.md             # 工程说明
├── 03_v2_USART/              # 串口通信与日志
│   ├── Components/           # 日志和按键组件
│   ├── Core/                 # 核心代码
│   ├── Drivers/              # HAL驱动
│   ├── MDK-ARM/              # Keil工程
│   └── README.md             # 工程说明
├── 04_v2_SDIO/               # SD卡文件系统
│   ├── Components/           # 组件库
│   ├── Core/                 # 核心代码
│   ├── FATFS/                # FatFs文件系统
│   ├── Middlewares/          # 中间件
│   ├── Drivers/              # HAL驱动
│   ├── MDK-ARM/              # Keil工程
│   └── README.md             # 工程说明
├── 05_v2_W25QXX/             # SPI Flash存储
│   ├── Components/           # Flash驱动组件
│   ├── Core/                 # 核心代码
│   ├── Drivers/              # HAL驱动
│   ├── MDK-ARM/              # Keil工程
│   └── README.md             # 工程说明
├── 06_v2_ICM20608/           # 六轴传感器
│   ├── Components/           # 传感器驱动组件
│   ├── Core/                 # 核心代码
│   ├── Drivers/              # HAL驱动
│   ├── MDK-ARM/              # Keil工程
│   └── README.md             # 工程说明
├── 07_v2_AHT20/              # 温湿度传感器
│   ├── Components/           # 传感器驱动组件
│   ├── Core/                 # 核心代码
│   ├── Drivers/              # HAL驱动
│   ├── MDK-ARM/              # Keil工程
│   └── README.md             # 工程说明
└── README.md                 # 总体说明文档
```

## 开发指南

### 开发环境搭建
1. **安装Keil MDK-ARM**: 版本5.29或更高
2. **安装STM32CubeMX**: 用于配置外设和生成代码
3. **安装ST-Link驱动**: 用于程序下载和调试
4. **准备开发板**: STM32F407VET6 MainBoard V2.0

### 编译和下载流程
```bash
# 1. 打开Keil工程
# 双击对应工程目录下的 MDK-ARM/*.uvprojx 文件

# 2. 配置工程
# Project -> Options for Target -> Debug
# 选择ST-Link Debugger

# 3. 编译工程
# Project -> Build Target (F7)

# 4. 下载程序
# Flash -> Download (F8)

# 5. 运行调试
# Debug -> Start/Stop Debug Session (Ctrl+F5)
```

### 移植指南
如需移植到其他STM32F4系列芯片：
1. 修改STM32CubeMX配置文件(.ioc)
2. 重新生成代码
3. 检查引脚定义和时钟配置
4. 调整外设参数
5. 重新编译和测试

## 技术支持

### 常见问题解答
1. **编译错误**: 检查Keil版本和HAL库配置
2. **下载失败**: 检查ST-Link连接和驱动
3. **程序不运行**: 检查时钟配置和复位电路
4. **外设不工作**: 检查硬件连接和引脚配置

### 调试技巧
- 使用串口输出调试信息
- 利用Keil调试器观察变量
- 使用逻辑分析仪检查信号
- 添加LED指示程序状态

### 文档

**离线文档**: /doc目录包含开发板数据手册和驱动指南

**在线资源**:
- [STM32F407参考手册](https://www.st.com/resource/en/reference_manual/dm00031020.pdf)
- [STM32F4 HAL库文档](https://www.st.com/resource/en/user_manual/dm00105879.pdf)
- [STM32CubeMX用户手册](https://www.st.com/resource/en/user_manual/dm00104712.pdf)

### 贡献

欢迎提交代码贡献和改进建议：
1. Fork本项目
2. 创建功能分支
3. 提交代码更改
4. 发起Pull Request

### 版权

版权 (c) 2025 MCU Electronics Technology 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称"软件"）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是"如此"提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

如有技术问题或建议，请通过以下方式联系：
- 项目主页: https://gitee.com/Ahypnis
