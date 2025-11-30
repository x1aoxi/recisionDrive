# 00_Template - STM32F407VET6 项目模板

[![HAL](https://img.shields.io/badge/HAL-STM32F4-brightgreen.svg)](/Drivers) [![Keil](https://img.shields.io/badge/IDE-Keil_MDK-blue.svg)](https://www.keil.com/) 

## 项目简介

00_Template是STM32F407VET6开发板的基础项目模板，提供了完整的工程框架和任务调度器。该模板包含了STM32 HAL库的基本配置、时钟配置、GPIO初始化等基础功能，为后续的功能开发提供了标准的起始框架。

## 主要特性

- **基础系统配置**: STM32F407VET6基本系统初始化
- **时钟配置**: 系统时钟配置为168MHz
- **任务调度器**: 基于SysTick的简单任务调度框架
- **模块化设计**: 清晰的目录结构和代码组织
- **Keil工程配置**: 预配置的Keil MDK-ARM工程文件

## 硬件连接

- **主控芯片**: STM32F407VET6
- **时钟源**: 外部8MHz晶振 + PLL倍频至168MHz
- **调试接口**: SWD (PA13/PA14)

## 目录结构

```
00_Template/
├── Core/
│   ├── Inc/                  # 核心头文件
│   │   ├── main.h
│   │   ├── stm32f4xx_hal_conf.h
│   │   └── stm32f4xx_it.h
│   └── Src/                  # 核心源文件
│       ├── main.c
│       ├── stm32f4xx_hal_msp.c
│       ├── stm32f4xx_it.c
│       └── system_stm32f4xx.c
├── Drivers/
│   ├── CMSIS/                # CMSIS库
│   └── STM32F4xx_HAL_Driver/ # HAL驱动库
├── MDK-ARM/
│   ├── Template.uvprojx      # Keil工程文件
│   └── startup_stm32f407xx.s # 启动文件
├── User/                     # 用户代码
│   ├── MyDefine.h           # 用户定义头文件
│   ├── Scheduler.c          # 任务调度器实现
│   ├── Scheduler.h          # 任务调度器头文件
│   ├── Scheduler_Task.c     # 任务实现
│   ├── Scheduler_Task.h     # 任务头文件
│   ├── App/                 # 应用层代码
│   ├── Driver/              # 驱动层代码
│   └── Module/              # 模块层代码
├── Template.ioc             # STM32CubeMX配置文件
└── keilkilll.bat           # Keil进程清理脚本
```

## 使用方法

### 快速开始

1. **打开工程**
   ```bash
   # 使用Keil MDK-ARM打开工程文件
   双击 MDK-ARM/Template.uvprojx
   ```

2. **编译工程**
   ```bash
   # 在Keil中按F7或点击编译按钮
   Project -> Build Target
   ```

3. **下载程序**
   ```bash
   # 连接ST-Link调试器
   # 在Keil中按F8下载程序
   Flash -> Download
   ```

### 代码示例

#### 主函数

```c
#include "main.h"
#include "Scheduler.h"

int main(void)
{
    /* HAL库初始化 */
    HAL_Init();
    
    /* 系统时钟配置 */
    SystemClock_Config();
    
    /* GPIO初始化 */
    MX_GPIO_Init();
    
    /* 用户初始化 */
    User_Init();
    
    /* 进入主循环 */
    while (1)
    {
        /* 任务调度 */
        Scheduler_Run();
    }
}
```

#### 任务调度器使用

```c
#include "Scheduler.h"

/* 任务函数示例 */
void Task_Example(void)
{
    static uint32_t counter = 0;
    counter++;
    
    /* 任务逻辑 */
    // 在这里添加你的任务代码
}

/* 任务初始化 */
void User_Init(void)
{
    /* 注册任务 */
    Scheduler_Add_Task(Task_Example, 1000); // 1000ms执行一次
}
```

## 配置说明

### 时钟配置
- **外部晶振**: 8MHz
- **系统时钟**: 168MHz
- **AHB时钟**: 168MHz  
- **APB1时钟**: 42MHz
- **APB2时钟**: 84MHz

### 编译器配置
- **IDE**: Keil MDK-ARM 5.29+
- **编译器**: ARMCC
- **优化等级**: -O1
- **调试信息**: 包含

## 开发指南

### 添加新功能模块

1. 在`User/App/`目录下创建应用模块
2. 在`User/Driver/`目录下创建驱动模块  
3. 在`User/Module/`目录下创建功能模块
4. 在`Scheduler_Task.c`中注册新任务

### 移植到其他芯片

1. 使用STM32CubeMX重新配置芯片型号
2. 修改工程文件中的芯片定义
3. 调整时钟配置参数
4. 重新生成代码

## 注意事项

- 确保外部8MHz晶振正常工作
- ST-Link连接正确
- Keil版本支持STM32F407VET6
- 注意任务执行时间，避免影响调度

## 技术支持

如有问题请参考：
- STM32F407参考手册
- STM32 HAL库文档
- Keil MDK-ARM用户手册

## 版权信息

版权 (c) 2025 MCU Electronics Technology

MIT License - 详见LICENSE文件
