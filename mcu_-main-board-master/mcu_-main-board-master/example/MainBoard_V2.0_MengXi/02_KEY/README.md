# 02_KEY - 按键输入检测示例

[![HAL](https://img.shields.io/badge/HAL-STM32F4-brightgreen.svg)](/Drivers) [![Keil](https://img.shields.io/badge/IDE-Keil_MDK-blue.svg)](https://www.keil.com/) 

## 项目简介

02_KEY是STM32F407VET6开发板的按键输入检测示例工程，演示如何使用GPIO读取按键状态，并实现按键防抖、按键事件检测等功能。该项目结合LED控制，展示了按键与LED的交互控制，是学习GPIO输入和事件处理的经典案例。

## 主要特性

- **按键状态检测**: 实时检测按键的按下和释放状态
- **硬件防抖**: 通过延时消除按键抖动
- **软件防抖**: 基于状态机的按键去抖算法
- **按键事件**: 支持按键按下、释放、长按等事件
- **LED交互**: 按键控制LED状态变化
- **模块化设计**: 独立的按键处理模块

## 硬件连接

| 功能 | 引脚 | 说明 |
|------|------|------|
| KEY1 | PA0 | 按键1，内部上拉，低电平有效 |
| KEY2 | PA1 | 按键2，内部上拉，低电平有效 |
| KEY3 | PA2 | 按键3，内部上拉，低电平有效 |
| KEY4 | PA3 | 按键4，内部上拉，低电平有效 |
| LED1 | PC0 | LED1，低电平点亮 |
| LED2 | PC1 | LED2，低电平点亮 |
| LED3 | PC2 | LED3，低电平点亮 |
| LED4 | PC3 | LED4，低电平点亮 |

**按键电路**: 使用内部上拉电阻，按键按下时引脚为低电平。

## 功能说明

### 按键检测功能
- **实时检测**: 周期性检测按键状态
- **防抖处理**: 消除按键机械抖动
- **状态识别**: 区分按下、释放、长按状态

### 按键事件类型
- **单击事件**: 按键快速按下并释放
- **长按事件**: 按键持续按下超过设定时间
- **释放事件**: 按键从按下状态释放

### LED控制交互
- 按键1控制LED1状态切换
- 按键2控制LED2状态切换  
- 按键3控制LED3状态切换
- 按键4控制LED4状态切换

## 目录结构

```
02_KEY/
├── Core/
│   ├── Inc/                  # 核心头文件
│   └── Src/                  # 核心源文件
├── Drivers/
│   ├── CMSIS/                # CMSIS库
│   └── STM32F4xx_HAL_Driver/ # HAL驱动库
├── MDK-ARM/
│   ├── 02_KEY.uvprojx        # Keil工程文件
│   └── startup_stm32f407xx.s # 启动文件
├── User/
│   ├── MyDefine.h            # 用户定义
│   ├── Scheduler.c           # 任务调度器
│   ├── Scheduler.h
│   ├── Scheduler_Task.c      # 任务实现
│   ├── Scheduler_Task.h
│   ├── App/
│   │   ├── key_app.c         # 按键应用实现
│   │   ├── key_app.h         # 按键应用头文件
│   │   ├── led_app.c         # LED应用实现
│   │   └── led_app.h         # LED应用头文件
│   ├── Driver/               # 驱动层
│   └── Module/               # 模块层
├── 02_KEY.ioc               # CubeMX配置文件
└── keilkilll.bat            # Keil进程清理脚本
```

## 使用方法

### 快速开始

1. **打开工程**
   ```bash
   双击 MDK-ARM/02_KEY.uvprojx
   ```

2. **编译下载**
   ```bash
   # 编译工程 (F7)
   # 下载程序 (F8)
   ```

3. **测试按键**
   - 按下KEY1，观察LED1状态变化
   - 按下KEY2，观察LED2状态变化
   - 按下其他按键，观察对应LED变化

### 代码示例

#### 按键初始化

```c
#include "key_app.h"

/* 按键状态结构体 */
typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
    uint8_t last_state;
    uint8_t current_state;
    uint32_t press_time;
    uint8_t debounce_count;
} key_t;

/* 按键实例 */
key_t keys[4] = {
    {GPIOA, GPIO_PIN_0, 1, 1, 0, 0},  // KEY1
    {GPIOA, GPIO_PIN_1, 1, 1, 0, 0},  // KEY2
    {GPIOA, GPIO_PIN_2, 1, 1, 0, 0},  // KEY3
    {GPIOA, GPIO_PIN_3, 1, 1, 0, 0}   // KEY4
};

void Key_Init(void)
{
    /* 按键GPIO已在CubeMX中配置为输入上拉 */
    /* 初始化按键状态 */
    for(int i = 0; i < 4; i++)
    {
        keys[i].last_state = 1;
        keys[i].current_state = 1;
        keys[i].press_time = 0;
        keys[i].debounce_count = 0;
    }
}
```

#### 按键扫描任务

```c
#define KEY_DEBOUNCE_TIME    20   // 防抖时间 20ms
#define KEY_LONG_PRESS_TIME  1000 // 长按时间 1000ms

void Key_Task(void)
{
    for(int i = 0; i < 4; i++)
    {
        /* 读取当前按键状态 */
        uint8_t key_state = HAL_GPIO_ReadPin(keys[i].port, keys[i].pin);
        
        /* 防抖处理 */
        if(key_state != keys[i].current_state)
        {
            keys[i].debounce_count++;
            if(keys[i].debounce_count >= KEY_DEBOUNCE_TIME)
            {
                keys[i].last_state = keys[i].current_state;
                keys[i].current_state = key_state;
                keys[i].debounce_count = 0;
                
                /* 按键状态变化处理 */
                Key_State_Change_Handler(i, key_state);
            }
        }
        else
        {
            keys[i].debounce_count = 0;
        }
        
        /* 长按检测 */
        if(keys[i].current_state == 0) // 按键按下
        {
            keys[i].press_time++;
            if(keys[i].press_time >= KEY_LONG_PRESS_TIME)
            {
                /* 长按事件处理 */
                Key_Long_Press_Handler(i);
                keys[i].press_time = 0; // 避免重复触发
            }
        }
        else
        {
            keys[i].press_time = 0;
        }
    }
}
```

#### 按键事件处理

```c
/* 按键状态变化处理 */
void Key_State_Change_Handler(uint8_t key_num, uint8_t state)
{
    if(state == 0) // 按键按下
    {
        Key_Press_Handler(key_num);
    }
    else // 按键释放
    {
        Key_Release_Handler(key_num);
    }
}

/* 按键按下处理 */
void Key_Press_Handler(uint8_t key_num)
{
    /* 切换对应LED状态 */
    extern uint8_t led_buf[4];
    led_buf[key_num] = !led_buf[key_num];
    
    printf("Key %d Pressed, LED %d: %s\r\n", 
           key_num + 1, key_num + 1, 
           led_buf[key_num] ? "ON" : "OFF");
}

/* 按键释放处理 */
void Key_Release_Handler(uint8_t key_num)
{
    printf("Key %d Released\r\n", key_num + 1);
}

/* 长按处理 */
void Key_Long_Press_Handler(uint8_t key_num)
{
    /* 长按时所有LED状态切换 */
    extern uint8_t led_buf[4];
    for(int i = 0; i < 4; i++)
    {
        led_buf[i] = !led_buf[i];
    }
    
    printf("Key %d Long Pressed, All LEDs Toggle\r\n", key_num + 1);
}
```

#### 主函数实现

```c
#include "main.h"
#include "gpio.h"
#include "Scheduler.h"
#include "key_app.h"
#include "led_app.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    
    /* 初始化 */
    Key_Init();
    Led_Init();
    
    while (1)
    {
        /* 任务调度 */
        Scheduler_Run();
    }
}
```

## API参考

### 函数列表

```c
/* 按键初始化 */
void Key_Init(void);

/* 按键扫描任务 */
void Key_Task(void);

/* 读取按键状态 */
uint8_t Key_Read(uint8_t key_num);

/* 按键事件注册 */
void Key_Register_Event(uint8_t key_num, void (*callback)(uint8_t));

/* 按键状态变化处理 */
void Key_State_Change_Handler(uint8_t key_num, uint8_t state);

/* 按键按下处理 */
void Key_Press_Handler(uint8_t key_num);

/* 按键释放处理 */  
void Key_Release_Handler(uint8_t key_num);

/* 长按处理 */
void Key_Long_Press_Handler(uint8_t key_num);
```

### 宏定义

```c
#define KEY_COUNT           4      // 按键数量
#define KEY_DEBOUNCE_TIME   20     // 防抖时间(ms)
#define KEY_LONG_PRESS_TIME 1000   // 长按时间(ms)

#define KEY1_PORT   GPIOA
#define KEY1_PIN    GPIO_PIN_0

#define KEY2_PORT   GPIOA  
#define KEY2_PIN    GPIO_PIN_1

#define KEY3_PORT   GPIOA
#define KEY3_PIN    GPIO_PIN_2

#define KEY4_PORT   GPIOA
#define KEY4_PIN    GPIO_PIN_3
```

## 配置选项

### 按键参数配置
```c
/* 在key_app.h中修改以下参数 */
#define KEY_DEBOUNCE_TIME    20    // 防抖时间，根据按键特性调整
#define KEY_LONG_PRESS_TIME  1000  // 长按时间，可根据需要修改
#define KEY_SCAN_PERIOD      10    // 按键扫描周期(ms)
```

### GPIO配置
- 按键引脚配置为输入模式
- 启用内部上拉电阻
- 无需配置中断

## 扩展功能

### 按键组合检测

```c
/* 检测多个按键同时按下 */
uint8_t Key_Combination_Check(uint8_t key_mask)
{
    uint8_t pressed_keys = 0;
    
    for(int i = 0; i < 4; i++)
    {
        if((key_mask & (1 << i)) && (keys[i].current_state == 0))
        {
            pressed_keys |= (1 << i);
        }
    }
    
    return (pressed_keys == key_mask);
}
```

### 双击检测

```c
#define DOUBLE_CLICK_TIME   300  // 双击时间间隔

void Key_Double_Click_Handler(uint8_t key_num)
{
    static uint32_t last_click_time[4] = {0};
    uint32_t current_time = HAL_GetTick();
    
    if((current_time - last_click_time[key_num]) < DOUBLE_CLICK_TIME)
    {
        /* 双击事件 */
        printf("Key %d Double Clicked\r\n", key_num + 1);
        last_click_time[key_num] = 0; // 重置
    }
    else
    {
        last_click_time[key_num] = current_time;
    }
}
```

### 按键状态机

```c
typedef enum {
    KEY_STATE_IDLE,
    KEY_STATE_DEBOUNCE,
    KEY_STATE_PRESSED,
    KEY_STATE_LONG_PRESS
} key_state_enum;

/* 基于状态机的按键处理 */
void Key_State_Machine(uint8_t key_num)
{
    static key_state_enum state[4] = {KEY_STATE_IDLE};
    static uint32_t state_time[4] = {0};
    
    uint8_t key_val = HAL_GPIO_ReadPin(keys[key_num].port, keys[key_num].pin);
    
    switch(state[key_num])
    {
        case KEY_STATE_IDLE:
            if(key_val == 0)
            {
                state[key_num] = KEY_STATE_DEBOUNCE;
                state_time[key_num] = HAL_GetTick();
            }
            break;
            
        case KEY_STATE_DEBOUNCE:
            if(key_val == 0)
            {
                if(HAL_GetTick() - state_time[key_num] > KEY_DEBOUNCE_TIME)
                {
                    state[key_num] = KEY_STATE_PRESSED;
                    Key_Press_Handler(key_num);
                }
            }
            else
            {
                state[key_num] = KEY_STATE_IDLE;
            }
            break;
            
        case KEY_STATE_PRESSED:
            if(key_val == 1)
            {
                state[key_num] = KEY_STATE_IDLE;
                Key_Release_Handler(key_num);
            }
            else if(HAL_GetTick() - state_time[key_num] > KEY_LONG_PRESS_TIME)
            {
                state[key_num] = KEY_STATE_LONG_PRESS;
                Key_Long_Press_Handler(key_num);
            }
            break;
            
        case KEY_STATE_LONG_PRESS:
            if(key_val == 1)
            {
                state[key_num] = KEY_STATE_IDLE;
                Key_Release_Handler(key_num);
            }
            break;
    }
}
```

## 故障排除

### 常见问题

1. **按键无响应**
   - 检查GPIO配置是否正确
   - 确认上拉电阻是否启用
   - 检查按键硬件连接

2. **按键抖动**
   - 增加防抖时间
   - 检查硬件滤波电路
   - 使用软件防抖算法

3. **误触发**
   - 调整防抖参数
   - 检查电源纹波
   - 优化PCB布线

### 调试方法

1. 使用串口输出按键状态
2. 使用示波器观察按键信号
3. 添加LED指示按键状态

## 学习要点

1. **GPIO输入**: 掌握GPIO输入模式配置和读取方法
2. **防抖算法**: 理解硬件抖动问题和软件解决方案
3. **事件驱动**: 学习基于事件的程序设计模式
4. **状态机**: 掌握状态机在按键处理中的应用

## 版权信息

版权 (c) 2025 MCU Electronics Technology

MIT License - 详见LICENSE文件
