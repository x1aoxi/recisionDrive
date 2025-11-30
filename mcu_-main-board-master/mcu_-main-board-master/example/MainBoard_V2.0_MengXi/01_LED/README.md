# 01_LED - LED灯控制示例

[![HAL](https://img.shields.io/badge/HAL-STM32F4-brightgreen.svg)](/Drivers) [![Keil](https://img.shields.io/badge/IDE-Keil_MDK-blue.svg)](https://www.keil.com/) 

## 项目简介

01_LED是STM32F407VET6开发板的LED控制示例工程，演示如何使用GPIO控制LED灯的亮灭。该项目实现了基本的LED闪烁功能，并提供了灵活的LED控制接口，适合初学者学习GPIO的基本操作。

## 主要特性

- **GPIO控制**: 使用HAL库GPIO接口控制LED
- **LED状态管理**: 支持LED的亮、灭、闪烁控制
- **任务调度**: 基于Scheduler的周期性LED控制
- **模块化设计**: 独立的LED应用模块
- **易于扩展**: 支持多个LED的独立控制

## 硬件连接

| 功能 | 引脚 | 说明 |
|------|------|------|
| LED1 | PC0 | 低电平点亮 |
| LED2 | PC1 | 低电平点亮 |
| LED3 | PC2 | 低电平点亮 |
| LED4 | PC3 | 低电平点亮 |

**注意**: LED为低电平有效，即GPIO输出低电平时LED点亮。

## 功能说明

### LED显示功能
- 支持4个LED的独立控制
- 可设置LED的亮灭状态
- 支持LED闪烁模式

### LED控制模式
- **静态显示**: LED保持固定状态
- **动态闪烁**: LED周期性闪烁
- **流水灯**: LED依次点亮效果

## 目录结构

```
01_LED/
├── Core/
│   ├── Inc/                  # 核心头文件
│   └── Src/                  # 核心源文件
├── Drivers/
│   ├── CMSIS/                # CMSIS库
│   └── STM32F4xx_HAL_Driver/ # HAL驱动库
├── MDK-ARM/
│   ├── 01_LED.uvprojx        # Keil工程文件
│   └── startup_stm32f407xx.s # 启动文件
├── User/
│   ├── MyDefine.h            # 用户定义
│   ├── Scheduler.c           # 任务调度器
│   ├── Scheduler.h
│   ├── Scheduler_Task.c      # 任务实现
│   ├── Scheduler_Task.h
│   └── App/
│       ├── led_app.c         # LED应用实现
│       └── led_app.h         # LED应用头文件
├── 01_LED.ioc               # CubeMX配置文件
└── keilkilll.bat            # Keil进程清理脚本
```

## 使用方法

### 快速开始

1. **打开工程**
   ```bash
   双击 MDK-ARM/01_LED.uvprojx
   ```

2. **编译下载**
   ```bash
   # 编译工程 (F7)
   # 下载程序 (F8)
   ```

3. **观察现象**
   - LED灯按照预设模式闪烁
   - 可通过修改代码改变闪烁模式

### 代码示例

#### LED初始化

```c
#include "led_app.h"

/* LED缓冲区：0-灭，1-亮 */
uint8_t led_buf[4] = {0, 1, 1, 0};

void Led_Init()
{
    /* 初始化LED状态 */
    Led_Display(led_buf, 0);
}
```

#### LED任务处理

```c
void Led_Task()
{
    /* 更新LED显示 */
    Led_Display(led_buf, 1);
}
```

#### 主函数实现

```c
#include "main.h"
#include "gpio.h"
#include "Scheduler.h"
#include "led_app.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    
    /* LED初始化 */
    Led_Init();
    
    while (1)
    {
        /* 任务调度 */
        Scheduler_Run();
    }
}
```

#### 自定义LED控制

```c
/* 设置单个LED状态 */
void Set_LED(uint8_t led_num, uint8_t state)
{
    if(led_num < 4)
    {
        led_buf[led_num] = state;
    }
}

/* 流水灯效果 */
void LED_Flowing_Light(void)
{
    static uint8_t step = 0;
    
    /* 清空所有LED */
    memset(led_buf, 0, sizeof(led_buf));
    
    /* 点亮当前LED */
    led_buf[step] = 1;
    
    /* 下一个LED */
    step++;
    if(step >= 4) step = 0;
    
    Led_Display(led_buf, 0);
}

/* LED闪烁 */
void LED_Blink_All(void)
{
    static uint8_t blink_state = 0;
    
    /* 所有LED同时闪烁 */
    for(int i = 0; i < 4; i++)
    {
        led_buf[i] = blink_state;
    }
    
    blink_state = !blink_state;
    Led_Display(led_buf, 0);
}
```

## API参考

### 函数列表

```c
/* LED初始化 */
void Led_Init(void);

/* LED任务处理 */
void Led_Task(void);

/* LED显示函数 */
void Led_Display(uint8_t *buf, uint8_t mode);

/* 设置单个LED状态 */
void Set_LED(uint8_t led_num, uint8_t state);
```

### 参数说明

- `led_num`: LED编号 (0-3)
- `state`: LED状态 (0-灭, 1-亮)  
- `buf`: LED状态缓冲区
- `mode`: 显示模式 (0-静态, 1-动态)

## 配置选项

### GPIO配置
```c
/* GPIO引脚定义 */
#define LED1_PIN    GPIO_PIN_0
#define LED1_PORT   GPIOC

#define LED2_PIN    GPIO_PIN_1  
#define LED2_PORT   GPIOC

#define LED3_PIN    GPIO_PIN_2
#define LED3_PORT   GPIOC

#define LED4_PIN    GPIO_PIN_3
#define LED4_PORT   GPIOC
```

### 任务调度配置
```c
/* LED任务执行周期 */
#define LED_TASK_PERIOD    100  // 100ms
```

## 扩展功能

### 添加更多LED

1. 在CubeMX中配置新的GPIO引脚
2. 修改`led_app.h`中的引脚定义
3. 增加LED缓冲区大小
4. 更新LED显示函数

### PWM调光

1. 配置定时器PWM输出
2. 替换GPIO控制为PWM输出
3. 实现亮度渐变效果

### 呼吸灯效果

```c
void LED_Breathing_Effect(void)
{
    static uint8_t brightness = 0;
    static int8_t direction = 1;
    
    brightness += direction;
    
    if(brightness >= 100) direction = -1;
    if(brightness <= 0) direction = 1;
    
    /* 使用PWM实现亮度控制 */
    // Set_LED_Brightness(brightness);
}
```

## 故障排除

### 常见问题

1. **LED不亮**
   - 检查硬件连接
   - 确认GPIO配置正确
   - 检查LED极性

2. **LED不闪烁**
   - 检查任务调度器是否运行
   - 确认任务周期设置
   - 检查LED_Task是否被调用

3. **闪烁频率不对**
   - 调整任务执行周期
   - 检查系统时钟配置

### 调试方法

1. 使用逻辑分析仪观察GPIO波形
2. 在关键函数处设置断点
3. 添加串口调试信息输出

## 学习要点

1. **GPIO基本操作**: 学习GPIO的配置和控制方法
2. **任务调度**: 理解基于时间的任务调度机制  
3. **模块化编程**: 学习如何组织和管理代码结构
4. **状态机设计**: LED控制中的状态管理

## 版权信息

版权 (c) 2025 MCU Electronics Technology

MIT License - 详见LICENSE文件
