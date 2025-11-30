# 03_UART - 串口通信示例

[![HAL](https://img.shields.io/badge/HAL-STM32F4-brightgreen.svg)](/Drivers) [![Keil](https://img.shields.io/badge/IDE-Keil_MDK-blue.svg)](https://www.keil.com/) 

## 项目简介

03_UART是STM32F407VET6开发板的串口通信示例工程，演示如何使用UART进行数据收发。该项目实现了基于DMA的高效串口通信，支持环形缓冲区数据管理、printf重定向、数据解析等功能，并结合按键和LED实现人机交互。

## 主要特性

- **UART通信**: 基于HAL库的UART1通信接口
- **DMA传输**: 使用DMA实现高效的数据收发
- **环形缓冲区**: 实现数据的缓存和管理
- **printf重定向**: 支持标准printf函数输出
- **数据解析**: 接收数据的解析和处理
- **交互控制**: 结合按键和LED的串口控制
- **中断处理**: UART空闲中断和DMA中断处理

## 硬件连接

| 功能 | 引脚 | 说明 |
|------|------|------|
| UART1_TX | PA9 | 串口发送引脚 |
| UART1_RX | PA10 | 串口接收引脚 |
| KEY1-KEY4 | PA0-PA3 | 按键输入 |
| LED1-LED4 | PC0-PC3 | LED指示 |

**串口参数**:
- 波特率: 115200
- 数据位: 8
- 停止位: 1  
- 校验位: 无
- 硬件流控: 无

## 功能说明

### 串口通信功能
- **数据发送**: 支持字符串和数组数据发送
- **数据接收**: DMA自动接收数据到缓冲区
- **空闲检测**: 利用UART空闲中断检测数据帧结束
- **printf支持**: 重定向printf到串口输出

### 数据管理
- **环形缓冲区**: 高效的数据存储和读取
- **数据长度检测**: 实时检测缓冲区数据长度
- **数据解析**: 自定义数据协议解析

### 交互功能
- 按键状态通过串口输出
- LED状态通过串口控制
- 串口命令控制系统行为

## 目录结构

```
03_UART/
├── Core/
│   ├── Inc/                  # 核心头文件
│   │   ├── main.h
│   │   ├── usart.h           # UART配置头文件
│   │   └── stm32f4xx_it.h    # 中断处理头文件
│   └── Src/                  # 核心源文件
│       ├── main.c
│       ├── usart.c           # UART配置源文件
│       └── stm32f4xx_it.c    # 中断处理源文件
├── Drivers/
│   ├── CMSIS/                # CMSIS库
│   └── STM32F4xx_HAL_Driver/ # HAL驱动库
├── MDK-ARM/
│   ├── 03_UART.uvprojx       # Keil工程文件
│   └── startup_stm32f407xx.s # 启动文件
├── User/
│   ├── MyDefine.h            # 用户定义
│   ├── Scheduler.c           # 任务调度器
│   ├── Scheduler.h
│   ├── Scheduler_Task.c      # 任务实现
│   ├── Scheduler_Task.h
│   └── App/
│       ├── uart_app.c        # UART应用实现
│       ├── uart_app.h        # UART应用头文件
│       ├── key_app.c         # 按键应用
│       ├── key_app.h
│       ├── led_app.c         # LED应用
│       └── led_app.h
├── 03_UART.ioc              # CubeMX配置文件
└── keilkilll.bat            # Keil进程清理脚本
```

## 使用方法

### 快速开始

1. **硬件连接**
   ```
   STM32开发板 <-> USB转TTL模块
   PA9(TX)     <-> RX
   PA10(RX)    <-> TX  
   GND         <-> GND
   ```

2. **打开串口工具**
   - 波特率: 115200
   - 数据位: 8
   - 停止位: 1
   - 校验: 无

3. **编译下载**
   ```bash
   # 打开Keil工程
   双击 MDK-ARM/03_UART.uvprojx
   # 编译工程 (F7)
   # 下载程序 (F8)
   ```

4. **观察现象**
   - 串口工具显示系统启动信息
   - 按下按键，串口输出按键状态
   - 发送命令控制LED状态

### 代码示例

#### UART初始化

```c
#include "uart_app.h"
#include "usart.h"

/* 串口1缓冲区定义 */
uint8_t uart1_rx_dma_buffer[BUFFER_SIZE];           // DMA接收缓冲区
uint8_t uart1_ring_buffer_input[BUFFER_SIZE];       // 环形缓冲区数组
struct rt_ringbuffer uart1_ring_buffer;             // 环形缓冲区结构
uint8_t uart1_data_buffer[BUFFER_SIZE];             // 数据处理缓冲区

void Uart_Init(void)
{
    /* 初始化环形缓冲区 */
    rt_ringbuffer_init(&uart1_ring_buffer, uart1_ring_buffer_input, BUFFER_SIZE);
    
    /* 启动DMA接收 */
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart1_rx_dma_buffer, BUFFER_SIZE);
    
    /* 关闭DMA半满中断 */
    __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
    
    printf("UART1 Initialized, Baud Rate: 115200\r\n");
}
```

#### 数据接收处理

```c
/* UART接收完成回调函数 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart->Instance == USART1)
    {
        /* 将接收到的数据放入环形缓冲区 */
        rt_ringbuffer_put(&uart1_ring_buffer, uart1_rx_dma_buffer, Size);
        
        /* 重新启动DMA接收 */
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart1_rx_dma_buffer, BUFFER_SIZE);
    }
}

/* UART数据处理任务 */
void Uart1_Task(void)
{
    uint16_t uart_data_len = rt_ringbuffer_data_len(&uart1_ring_buffer);
    
    if(uart_data_len > 0)
    {
        /* 从环形缓冲区取出数据 */
        rt_ringbuffer_get(&uart1_ring_buffer, uart1_data_buffer, uart_data_len);
        uart1_data_buffer[uart_data_len] = '\\0';
        
        /* 输出接收到的数据 */
        printf("UART1 Received: %s\\r\\n", uart1_data_buffer);
        
        /* 数据解析处理 */
        Uart_Data_Parse((char*)uart1_data_buffer, uart_data_len);
        
        /* 清空数据缓冲区 */
        memset(uart1_data_buffer, 0, uart_data_len);
    }
}
```

#### printf重定向

```c
#include "stdio.h"

/* 重定向printf到UART1 */
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

int fgetc(FILE *f)
{
    uint8_t ch;
    HAL_UART_Receive(&huart1, &ch, 1, HAL_MAX_DELAY);
    return ch;
}

/* 自定义printf函数 */
void Uart_Printf(UART_HandleTypeDef *huart, const char *format, ...)
{
    char buffer[256];
    va_list args;
    
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    HAL_UART_Transmit(huart, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}
```

#### 数据解析示例

```c
/* 简单的命令解析 */
void Uart_Data_Parse(char *data, uint16_t len)
{
    /* LED控制命令: LED1_ON, LED1_OFF, LED2_ON, LED2_OFF等 */
    if(strncmp(data, "LED", 3) == 0)
    {
        uint8_t led_num = data[3] - '1'; // 获取LED编号
        
        if(led_num < 4)
        {
            if(strstr(data, "_ON"))
            {
                Set_LED(led_num, 1);
                printf("LED%d ON\\r\\n", led_num + 1);
            }
            else if(strstr(data, "_OFF"))
            {
                Set_LED(led_num, 0);
                printf("LED%d OFF\\r\\n", led_num + 1);
            }
        }
    }
    /* 系统信息查询 */
    else if(strncmp(data, "STATUS", 6) == 0)
    {
        printf("System Status:\\r\\n");
        printf("CPU: STM32F407VET6\\r\\n");
        printf("Clock: %ldMHz\\r\\n", HAL_RCC_GetSysClockFreq() / 1000000);
        printf("Uptime: %ld ms\\r\\n", HAL_GetTick());
    }
    /* 帮助信息 */
    else if(strncmp(data, "HELP", 4) == 0)
    {
        printf("Available Commands:\\r\\n");
        printf("LED1_ON/LED1_OFF - Control LED1\\r\\n");
        printf("LED2_ON/LED2_OFF - Control LED2\\r\\n");
        printf("LED3_ON/LED3_OFF - Control LED3\\r\\n");
        printf("LED4_ON/LED4_OFF - Control LED4\\r\\n");
        printf("STATUS - Show system status\\r\\n");
        printf("HELP - Show this help\\r\\n");
    }
}
```

#### 主函数实现

```c
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "dma.h"
#include "Scheduler.h"
#include "uart_app.h"
#include "key_app.h"
#include "led_app.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART1_UART_Init();
    
    /* 应用初始化 */
    Uart_Init();
    Key_Init();
    Led_Init();
    
    printf("\\r\\n=== STM32F407VET6 UART Example ===\\r\\n");
    printf("System Started Successfully!\\r\\n");
    printf("Type 'HELP' for available commands.\\r\\n\\r\\n");
    
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
/* UART初始化 */
void Uart_Init(void);

/* UART1数据处理任务 */
void Uart1_Task(void);

/* 自定义printf */
void Uart_Printf(UART_HandleTypeDef *huart, const char *format, ...);

/* 数据解析 */
void Uart_Data_Parse(char *data, uint16_t len);

/* 发送字符串 */
HAL_StatusTypeDef Uart_Send_String(UART_HandleTypeDef *huart, char *str);

/* 发送数据 */
HAL_StatusTypeDef Uart_Send_Data(UART_HandleTypeDef *huart, uint8_t *data, uint16_t len);
```

### 环形缓冲区API

```c
/* 初始化环形缓冲区 */
void rt_ringbuffer_init(struct rt_ringbuffer *rb, rt_uint8_t *pool, rt_int16_t size);

/* 向环形缓冲区写入数据 */
rt_size_t rt_ringbuffer_put(struct rt_ringbuffer *rb, const rt_uint8_t *ptr, rt_uint16_t length);

/* 从环形缓冲区读取数据 */
rt_size_t rt_ringbuffer_get(struct rt_ringbuffer *rb, rt_uint8_t *ptr, rt_uint16_t length);

/* 获取环形缓冲区数据长度 */
rt_size_t rt_ringbuffer_data_len(struct rt_ringbuffer *rb);
```

## 配置选项

### UART参数配置
```c
/* 在usart.c中配置 */
huart1.Instance = USART1;
huart1.Init.BaudRate = 115200;
huart1.Init.WordLength = UART_WORDLENGTH_8B;
huart1.Init.StopBits = UART_STOPBITS_1;
huart1.Init.Parity = UART_PARITY_NONE;
huart1.Init.Mode = UART_MODE_TX_RX;
huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
```

### 缓冲区大小配置
```c
/* 在uart_app.h中定义 */
#define BUFFER_SIZE     256    // 缓冲区大小，可根据需要调整
```

### DMA配置
```c
/* DMA配置 - 在dma.c中 */
hdma_usart1_rx.Instance = DMA2_Stream5;
hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
hdma_usart1_rx.Init.Mode = DMA_NORMAL;
hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
```

## 扩展功能

### 多串口通信

```c
/* 串口2初始化 */
void Uart2_Init(void)
{
    rt_ringbuffer_init(&uart2_ring_buffer, uart2_ring_buffer_input, BUFFER_SIZE);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, uart2_rx_dma_buffer, BUFFER_SIZE);
    __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
}

/* 串口2数据处理 */
void Uart2_Task(void)
{
    // 类似uart1的处理方式
}
```

### 数据校验

```c
/* 简单的校验和计算 */
uint8_t Calculate_Checksum(uint8_t *data, uint16_t len)
{
    uint8_t checksum = 0;
    for(uint16_t i = 0; i < len; i++)
    {
        checksum += data[i];
    }
    return checksum;
}

/* 带校验的数据发送 */
void Uart_Send_With_Checksum(uint8_t *data, uint16_t len)
{
    uint8_t checksum = Calculate_Checksum(data, len);
    
    HAL_UART_Transmit(&huart1, data, len, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1, &checksum, 1, HAL_MAX_DELAY);
}
```

### JSON格式数据处理

```c
/* 简单的JSON解析示例 */
void Parse_JSON_Command(char *json_str)
{
    /* 解析格式: {"led":1,"state":"on"} */
    char *led_pos = strstr(json_str, "\\"led\\":");
    char *state_pos = strstr(json_str, "\\"state\\":");
    
    if(led_pos && state_pos)
    {
        int led_num = atoi(led_pos + 6);
        char *state_start = strchr(state_pos + 8, '"') + 1;
        
        if(led_num >= 1 && led_num <= 4)
        {
            if(strncmp(state_start, "on", 2) == 0)
            {
                Set_LED(led_num - 1, 1);
            }
            else if(strncmp(state_start, "off", 3) == 0)
            {
                Set_LED(led_num - 1, 0);
            }
        }
    }
}
```

## 故障排除

### 常见问题

1. **无法接收数据**
   - 检查TX/RX引脚连接是否正确
   - 确认波特率设置一致
   - 检查DMA配置是否正确

2. **数据丢失**
   - 增加缓冲区大小
   - 检查数据处理速度
   - 确认中断优先级设置

3. **printf不输出**
   - 检查fputc函数是否正确重定向
   - 确认UART初始化完成
   - 检查缓冲区设置

### 调试方法

1. 使用示波器检查UART信号
2. 添加LED指示数据收发状态
3. 使用调试器观察缓冲区数据

## 性能优化

1. **DMA方式**: 减少CPU占用率
2. **环形缓冲区**: 提高数据处理效率
3. **中断优先级**: 合理设置中断优先级
4. **数据解析**: 优化解析算法

## 学习要点

1. **UART通信**: 掌握串口通信原理和配置
2. **DMA传输**: 理解DMA在数据传输中的应用
3. **环形缓冲区**: 学习高效的数据缓存方法
4. **中断处理**: 掌握UART中断处理机制
5. **数据协议**: 学习简单的通信协议设计

## 版权信息

版权 (c) 2025 MCU Electronics Technology

MIT License - 详见LICENSE文件
