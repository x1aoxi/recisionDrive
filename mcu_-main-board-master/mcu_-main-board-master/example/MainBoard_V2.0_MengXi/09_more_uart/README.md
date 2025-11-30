# 09_more_uart - 多串口通信示例

[![HAL](https://img.shields.io/badge/HAL-STM32F4-brightgreen.svg)](/Drivers) [![Keil](https://img.shields.io/badge/IDE-Keil_MDK-blue.svg)](https://www.keil.com/) 

## 项目简介

09_more_uart是STM32F407VET6开发板的多串口通信示例工程，演示了如何同时使用多个UART接口进行串口通信。该项目展示了多串口数据收发、串口间数据转发、不同波特率配置、DMA传输等功能，适用于需要多路串口通信的应用场景，如多设备通信、数据网关、串口调试等。

## 主要特性

- **多串口支持**: 同时使用UART1、UART2、UART3等多个串口
- **独立配置**: 每个串口可独立配置波特率、数据位、停止位
- **DMA传输**: 支持DMA方式减少CPU占用
- **环形缓冲**: 使用环形缓冲区管理收发数据
- **数据转发**: 串口间数据透明转发功能
- **协议解析**: 支持多种通信协议解析
- **状态监控**: 实时监控各串口通信状态
- **错误处理**: 完善的错误检测和恢复机制

## 硬件连接

### 多串口连接配置

| 串口 | TX引脚 | RX引脚 | 功能描述 |
|------|--------|--------|----------|
| UART1 | PA9 | PA10 | 调试串口/PC通信 |
| UART2 | PA2 | PA3 | 设备通信串口1 |
| UART3 | PB10 | PB11 | 设备通信串口2 |
| UART4 | PC10 | PC11 | 设备通信串口3 |
| UART5 | PC12 | PD2 | 设备通信串口4 |

### 外部设备连接示例

```
    STM32F407VET6                    外部设备
    ┌──────────────┐                ┌─────────────┐
    │ UART1(调试)  │ <──USB-TTL───> │ PC/调试工具  │
    │ PA9/PA10     │                └─────────────┘
    ├──────────────┤                
    │ UART2(GPS)   │ <─────────────> │ GPS模块     │
    │ PA2/PA3      │                │ 9600,8,N,1  │
    ├──────────────┤                └─────────────┘
    │ UART3(WiFi)  │ <─────────────> │ WiFi模块    │
    │ PB10/PB11    │                │ 115200,8,N,1│
    ├──────────────┤                └─────────────┘
    │ UART4(4G)    │ <─────────────> │ 4G模块      │
    │ PC10/PC11    │                │ 115200,8,N,1│
    ├──────────────┤                └─────────────┘
    │ UART5(传感器) │ <─────────────> │ 传感器模块   │
    │ PC12/PD2     │                │ 38400,8,N,1 │
    └──────────────┘                └─────────────┘
```

### 典型连接场景

| 应用场景 | 串口配置 | 设备类型 |
|----------|----------|----------|
| 物联网网关 | UART1: 调试, UART2: WiFi, UART3: 4G, UART4: LoRa | 通信模块 |
| 数据采集器 | UART1: 调试, UART2-5: 传感器 | 多传感器 |
| 串口调试器 | UART1: PC, UART2-5: 目标设备 | 调试设备 |

## 目录结构

```
09_more_uart/
├── Core/
│   ├── Inc/                     # 核心头文件
│   │   ├── main.h
│   │   ├── gpio.h
│   │   ├── usart.h
│   │   ├── dma.h
│   │   └── it.h
│   └── Src/                     # 核心源文件
│       ├── main.c
│       ├── gpio.c
│       ├── usart.c
│       ├── dma.c
│       └── it.c
├── Drivers/
│   ├── CMSIS/                   # CMSIS库
│   └── STM32F4xx_HAL_Driver/    # HAL驱动库
├── MDK-ARM/
│   ├── 09_more_uart.uvprojx     # Keil工程文件
│   └── startup_stm32f407xx.s    # 启动文件
├── User/
│   ├── MyDefine.h               # 用户定义
│   ├── Scheduler.c              # 任务调度器
│   ├── Scheduler.h
│   ├── Scheduler_Task.c         # 任务实现
│   ├── Scheduler_Task.h
│   ├── App/
│   │   ├── uart_manager.c       # 串口管理器
│   │   ├── uart_manager.h
│   │   ├── data_router.c        # 数据路由器
│   │   ├── data_router.h
│   │   ├── protocol_handler.c   # 协议处理器
│   │   └── protocol_handler.h
│   ├── Driver/
│   │   ├── multi_uart.c         # 多串口驱动
│   │   ├── multi_uart.h
│   │   ├── ring_buffer.c        # 环形缓冲区
│   │   └── ring_buffer.h
│   └── Module/
│       ├── gps_parser.c         # GPS解析模块
│       ├── gps_parser.h
│       ├── at_command.c         # AT命令模块
│       ├── at_command.h
│       ├── modbus_rtu.c         # Modbus RTU模块
│       └── modbus_rtu.h
├── 09_more_uart.ioc            # CubeMX配置文件
└── keilkilll.bat               # Keil进程清理脚本
```

## 使用方法

### 快速开始

1. **硬件连接**
   - 按照连接表连接各个串口设备
   - 确保供电和地线连接正确
   - 注意电平匹配(3.3V/5V)

2. **编译下载**
   ```bash
   # 打开Keil工程
   双击 MDK-ARM/09_more_uart.uvprojx
   # 编译工程 (F7)
   # 下载程序 (F8)
   ```

3. **运行测试**
   - 连接UART1到PC进行调试
   - 连接其他串口到对应设备
   - 发送命令测试多串口功能

### 代码示例

#### 多串口驱动实现

```c
#include "multi_uart.h"
#include "ring_buffer.h"

/* 串口配置结构体 */
typedef struct {
    UART_HandleTypeDef* huart;      // HAL句柄
    uint32_t baudrate;              // 波特率
    uint8_t databits;               // 数据位
    uint8_t stopbits;               // 停止位
    uint8_t parity;                 // 校验位
    uint8_t enable;                 // 使能标志
    char name[16];                  // 串口名称
} uart_config_t;

/* 串口数据结构体 */
typedef struct {
    uart_config_t config;           // 配置信息
    ring_buffer_t tx_buffer;        // 发送缓冲区
    ring_buffer_t rx_buffer;        // 接收缓冲区
    uint8_t tx_dma_buffer[256];     // DMA发送缓冲
    uint8_t rx_dma_buffer[256];     // DMA接收缓冲
    uint32_t tx_count;              // 发送计数
    uint32_t rx_count;              // 接收计数
    uint32_t error_count;           // 错误计数
    uint8_t tx_busy;                // 发送忙标志
} uart_instance_t;

/* 串口实例数组 */
uart_instance_t uart_instances[5];

/* 串口配置表 */
const uart_config_t default_configs[5] = {
    {&huart1, 115200, 8, 1, 0, 1, "DEBUG"},    // UART1: 调试串口
    {&huart2, 9600,   8, 1, 0, 1, "GPS"},      // UART2: GPS模块
    {&huart3, 115200, 8, 1, 0, 1, "WIFI"},     // UART3: WiFi模块
    {&huart4, 115200, 8, 1, 0, 1, "4G"},       // UART4: 4G模块
    {&huart5, 38400,  8, 1, 0, 1, "SENSOR"},   // UART5: 传感器
};

/* 多串口初始化 */
void Multi_UART_Init(void)
{
    printf("Multi-UART Initialization...\\r\\n");
    
    for(int i = 0; i < 5; i++)
    {
        /* 复制默认配置 */
        uart_instances[i].config = default_configs[i];
        
        /* 初始化环形缓冲区 */
        Ring_Buffer_Init(&uart_instances[i].tx_buffer, 1024);
        Ring_Buffer_Init(&uart_instances[i].rx_buffer, 1024);
        
        /* 初始化统计信息 */
        uart_instances[i].tx_count = 0;
        uart_instances[i].rx_count = 0;
        uart_instances[i].error_count = 0;
        uart_instances[i].tx_busy = 0;
        
        /* 启动DMA接收 */
        if(uart_instances[i].config.enable)
        {
            HAL_UART_Receive_DMA(uart_instances[i].config.huart, 
                                uart_instances[i].rx_dma_buffer, 256);
            
            printf("  %s: %d baud, %d-%d-%d, DMA enabled\\r\\n",
                   uart_instances[i].config.name,
                   uart_instances[i].config.baudrate,
                   uart_instances[i].config.databits,
                   uart_instances[i].config.parity,
                   uart_instances[i].config.stopbits);
        }
    }
    
    printf("Multi-UART Init Complete\\r\\n");
}

/* 串口发送数据 */
uint16_t Multi_UART_Send(uint8_t uart_id, uint8_t* data, uint16_t length)
{
    if(uart_id >= 5 || !uart_instances[uart_id].config.enable)
        return 0;
    
    uart_instance_t* uart = &uart_instances[uart_id];
    
    /* 数据写入发送缓冲区 */
    uint16_t written = Ring_Buffer_Write(&uart->tx_buffer, data, length);
    
    /* 启动DMA发送(如果当前不忙) */
    if(!uart->tx_busy && Ring_Buffer_Available(&uart->tx_buffer) > 0)
    {
        Multi_UART_Start_DMA_TX(uart_id);
    }
    
    return written;
}

/* 启动DMA发送 */
void Multi_UART_Start_DMA_TX(uint8_t uart_id)
{
    uart_instance_t* uart = &uart_instances[uart_id];
    
    if(uart->tx_busy) return;
    
    /* 从环形缓冲区读取数据到DMA缓冲区 */
    uint16_t length = Ring_Buffer_Read(&uart->tx_buffer, 
                                      uart->tx_dma_buffer, 256);
    
    if(length > 0)
    {
        uart->tx_busy = 1;
        HAL_UART_Transmit_DMA(uart->config.huart, uart->tx_dma_buffer, length);
    }
}

/* 串口接收数据 */
uint16_t Multi_UART_Receive(uint8_t uart_id, uint8_t* data, uint16_t max_length)
{
    if(uart_id >= 5 || !uart_instances[uart_id].config.enable)
        return 0;
    
    uart_instance_t* uart = &uart_instances[uart_id];
    
    /* 从接收缓冲区读取数据 */
    return Ring_Buffer_Read(&uart->rx_buffer, data, max_length);
}

/* 获取接收数据长度 */
uint16_t Multi_UART_Available(uint8_t uart_id)
{
    if(uart_id >= 5 || !uart_instances[uart_id].config.enable)
        return 0;
    
    return Ring_Buffer_Available(&uart_instances[uart_id].rx_buffer);
}

/* DMA发送完成回调 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    /* 查找对应的串口实例 */
    for(int i = 0; i < 5; i++)
    {
        if(uart_instances[i].config.huart == huart)
        {
            uart_instances[i].tx_busy = 0;
            uart_instances[i].tx_count++;
            
            /* 检查是否还有数据需要发送 */
            if(Ring_Buffer_Available(&uart_instances[i].tx_buffer) > 0)
            {
                Multi_UART_Start_DMA_TX(i);
            }
            break;
        }
    }
}

/* DMA接收完成回调 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* 查找对应的串口实例 */
    for(int i = 0; i < 5; i++)
    {
        if(uart_instances[i].config.huart == huart)
        {
            /* 将DMA缓冲区数据写入环形缓冲区 */
            Ring_Buffer_Write(&uart_instances[i].rx_buffer, 
                             uart_instances[i].rx_dma_buffer, 256);
            
            uart_instances[i].rx_count++;
            
            /* 重新启动DMA接收 */
            HAL_UART_Receive_DMA(huart, uart_instances[i].rx_dma_buffer, 256);
            break;
        }
    }
}

/* 串口错误回调 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    /* 查找对应的串口实例 */
    for(int i = 0; i < 5; i++)
    {
        if(uart_instances[i].config.huart == huart)
        {
            uart_instances[i].error_count++;
            uart_instances[i].tx_busy = 0;  // 清除发送忙标志
            
            /* 重新启动DMA接收 */
            HAL_UART_Receive_DMA(huart, uart_instances[i].rx_dma_buffer, 256);
            
            printf("UART%d Error: %ld\\r\\n", i+1, huart->ErrorCode);
            break;
        }
    }
}

/* 获取串口状态 */
uart_instance_t* Multi_UART_Get_Status(uint8_t uart_id)
{
    if(uart_id >= 5) return NULL;
    return &uart_instances[uart_id];
}
```

#### 数据路由器实现

```c
#include "data_router.h"

/* 路由规则结构体 */
typedef struct {
    uint8_t src_uart;               // 源串口
    uint8_t dst_uart;               // 目标串口
    uint8_t enable;                 // 使能标志
    uint8_t filter_enable;          // 过滤使能
    uint8_t filter_header[4];       // 过滤头
    uint8_t filter_length;          // 过滤头长度
} route_rule_t;

/* 路由表 */
#define MAX_ROUTES  10
route_rule_t route_table[MAX_ROUTES];
uint8_t route_count = 0;

/* 数据转发统计 */
typedef struct {
    uint32_t total_packets;         // 总包数
    uint32_t routed_packets;        // 已路由包数
    uint32_t dropped_packets;       // 丢弃包数
} route_stats_t;

route_stats_t route_stats;

/* 路由器初始化 */
void Data_Router_Init(void)
{
    memset(route_table, 0, sizeof(route_table));
    route_count = 0;
    
    memset(&route_stats, 0, sizeof(route_stats));
    
    printf("Data Router Initialized\\r\\n");
}

/* 添加路由规则 */
uint8_t Data_Router_Add_Rule(uint8_t src_uart, uint8_t dst_uart, 
                             uint8_t* filter_header, uint8_t filter_length)
{
    if(route_count >= MAX_ROUTES) return 0;
    
    route_rule_t* rule = &route_table[route_count];
    rule->src_uart = src_uart;
    rule->dst_uart = dst_uart;
    rule->enable = 1;
    rule->filter_enable = (filter_header != NULL && filter_length > 0);
    rule->filter_length = filter_length;
    
    if(rule->filter_enable)
    {
        memcpy(rule->filter_header, filter_header, 
               filter_length > 4 ? 4 : filter_length);
    }
    
    route_count++;
    
    printf("Route Added: UART%d -> UART%d", src_uart+1, dst_uart+1);
    if(rule->filter_enable)
    {
        printf(", Filter: ");
        for(int i = 0; i < rule->filter_length; i++)
        {
            printf("%02X ", rule->filter_header[i]);
        }
    }
    printf("\\r\\n");
    
    return 1;
}

/* 删除路由规则 */
void Data_Router_Remove_Rule(uint8_t index)
{
    if(index >= route_count) return;
    
    /* 向前移动数组元素 */
    for(int i = index; i < route_count - 1; i++)
    {
        route_table[i] = route_table[i + 1];
    }
    
    route_count--;
    printf("Route %d removed\\r\\n", index);
}

/* 检查过滤条件 */
uint8_t Data_Router_Check_Filter(route_rule_t* rule, uint8_t* data, uint16_t length)
{
    if(!rule->filter_enable) return 1;  // 无过滤，直接通过
    
    if(length < rule->filter_length) return 0;  // 数据长度不足
    
    /* 检查头部匹配 */
    return (memcmp(data, rule->filter_header, rule->filter_length) == 0);
}

/* 数据路由处理 */
void Data_Router_Process(void)
{
    uint8_t buffer[256];
    
    /* 遍历所有串口 */
    for(int uart_id = 0; uart_id < 5; uart_id++)
    {
        uint16_t available = Multi_UART_Available(uart_id);
        if(available == 0) continue;
        
        /* 读取数据 */
        uint16_t length = Multi_UART_Receive(uart_id, buffer, sizeof(buffer));
        if(length == 0) continue;
        
        route_stats.total_packets++;
        
        /* 查找匹配的路由规则 */
        uint8_t routed = 0;
        for(int i = 0; i < route_count; i++)
        {
            route_rule_t* rule = &route_table[i];
            
            if(!rule->enable || rule->src_uart != uart_id)
                continue;
            
            /* 检查过滤条件 */
            if(Data_Router_Check_Filter(rule, buffer, length))
            {
                /* 转发数据到目标串口 */
                uint16_t sent = Multi_UART_Send(rule->dst_uart, buffer, length);
                if(sent > 0)
                {
                    routed = 1;
                    route_stats.routed_packets++;
                    
                    printf("Route: UART%d -> UART%d (%d bytes)\\r\\n", 
                           uart_id+1, rule->dst_uart+1, sent);
                }
            }
        }
        
        if(!routed)
        {
            route_stats.dropped_packets++;
        }
    }
}

/* 获取路由统计 */
route_stats_t* Data_Router_Get_Stats(void)
{
    return &route_stats;
}

/* 显示路由表 */
void Data_Router_Show_Table(void)
{
    printf("\\r\\n=== Route Table ===\\r\\n");
    printf("Index | Src | Dst | Enable | Filter\\r\\n");
    printf("------|-----|-----|--------|-------\\r\\n");
    
    for(int i = 0; i < route_count; i++)
    {
        route_rule_t* rule = &route_table[i];
        printf("  %2d  | %2d  | %2d  |   %s   | ", 
               i, rule->src_uart+1, rule->dst_uart+1,
               rule->enable ? "Y" : "N");
        
        if(rule->filter_enable)
        {
            for(int j = 0; j < rule->filter_length; j++)
            {
                printf("%02X ", rule->filter_header[j]);
            }
        }
        else
        {
            printf("None");
        }
        printf("\\r\\n");
    }
    
    printf("\\r\\nStats:\\r\\n");
    printf("  Total: %ld\\r\\n", route_stats.total_packets);
    printf("  Routed: %ld\\r\\n", route_stats.routed_packets);
    printf("  Dropped: %ld\\r\\n", route_stats.dropped_packets);
    printf("==================\\r\\n");
}
```

#### AT命令处理模块

```c
#include "at_command.h"

/* AT命令结构体 */
typedef struct {
    char command[32];               // 命令字符串
    uint32_t timeout;               // 超时时间(ms)
    char expected_response[64];     // 期望响应
    uint8_t uart_id;                // 目标串口ID
} at_command_t;

/* AT命令队列 */
#define AT_QUEUE_SIZE   10
at_command_t at_queue[AT_QUEUE_SIZE];
uint8_t queue_head = 0;
uint8_t queue_tail = 0;
uint8_t queue_count = 0;

/* AT命令状态 */
typedef struct {
    uint8_t busy;                   // 忙状态
    uint32_t send_time;             // 发送时间
    at_command_t current_cmd;       // 当前命令
    char response_buffer[256];      // 响应缓冲区
    uint16_t response_length;       // 响应长度
} at_state_t;

at_state_t at_state;

/* AT命令模块初始化 */
void AT_Command_Init(void)
{
    queue_head = 0;
    queue_tail = 0;
    queue_count = 0;
    
    memset(&at_state, 0, sizeof(at_state));
    
    printf("AT Command Module Initialized\\r\\n");
}

/* 添加AT命令到队列 */
uint8_t AT_Command_Add(uint8_t uart_id, char* command, char* expected_response, uint32_t timeout)
{
    if(queue_count >= AT_QUEUE_SIZE) return 0;
    
    at_command_t* cmd = &at_queue[queue_tail];
    cmd->uart_id = uart_id;
    cmd->timeout = timeout;
    
    strncpy(cmd->command, command, sizeof(cmd->command) - 1);
    strncpy(cmd->expected_response, expected_response, sizeof(cmd->expected_response) - 1);
    
    queue_tail = (queue_tail + 1) % AT_QUEUE_SIZE;
    queue_count++;
    
    printf("AT Command Added: %s\\r\\n", command);
    return 1;
}

/* 发送AT命令 */
uint8_t AT_Command_Send(at_command_t* cmd)
{
    /* 发送命令 */
    uint16_t length = strlen(cmd->command);
    uint16_t sent = Multi_UART_Send(cmd->uart_id, (uint8_t*)cmd->command, length);
    
    if(sent > 0)
    {
        /* 添加回车换行 */
        Multi_UART_Send(cmd->uart_id, (uint8_t*)"\\r\\n", 2);
        
        /* 更新状态 */
        at_state.busy = 1;
        at_state.send_time = HAL_GetTick();
        at_state.current_cmd = *cmd;
        at_state.response_length = 0;
        
        printf("AT Sent: %s\\r\\n", cmd->command);
        return 1;
    }
    
    return 0;
}

/* AT命令处理任务 */
void AT_Command_Task(void)
{
    /* 如果当前没有命令在执行，从队列取下一个 */
    if(!at_state.busy && queue_count > 0)
    {
        at_command_t* cmd = &at_queue[queue_head];
        
        if(AT_Command_Send(cmd))
        {
            queue_head = (queue_head + 1) % AT_QUEUE_SIZE;
            queue_count--;
        }
    }
    
    /* 如果有命令在执行，检查响应 */
    if(at_state.busy)
    {
        AT_Command_Check_Response();
    }
}

/* 检查AT命令响应 */
void AT_Command_Check_Response(void)
{
    uint8_t uart_id = at_state.current_cmd.uart_id;
    uint16_t available = Multi_UART_Available(uart_id);
    
    if(available > 0)
    {
        /* 读取响应数据 */
        uint16_t space = sizeof(at_state.response_buffer) - at_state.response_length - 1;
        uint16_t read_len = Multi_UART_Receive(uart_id, 
            (uint8_t*)at_state.response_buffer + at_state.response_length, space);
        
        at_state.response_length += read_len;
        at_state.response_buffer[at_state.response_length] = '\\0';
        
        /* 检查期望响应 */
        if(strlen(at_state.current_cmd.expected_response) > 0)
        {
            if(strstr(at_state.response_buffer, at_state.current_cmd.expected_response))
            {
                printf("AT OK: %s\\r\\n", at_state.current_cmd.command);
                at_state.busy = 0;
                return;
            }
        }
    }
    
    /* 检查超时 */
    if(HAL_GetTick() - at_state.send_time > at_state.current_cmd.timeout)
    {
        printf("AT Timeout: %s\\r\\n", at_state.current_cmd.command);
        at_state.busy = 0;
    }
}

/* 获取AT响应 */
char* AT_Command_Get_Response(void)
{
    return at_state.response_buffer;
}
```

#### GPS数据解析模块

```c
#include "gps_parser.h"

/* GPS数据结构 */
typedef struct {
    float latitude;                 // 纬度
    float longitude;                // 经度
    float altitude;                 // 海拔
    float speed;                    // 速度(km/h)
    float heading;                  // 航向角
    uint8_t satellites;             // 卫星数
    uint8_t fix_quality;            // 定位质量
    char time[16];                  // UTC时间
    char date[16];                  // 日期
    uint8_t data_valid;             // 数据有效标志
} gps_data_t;

gps_data_t gps_data;

/* NMEA数据缓冲区 */
char nmea_buffer[256];
uint16_t nmea_index = 0;

/* GPS解析器初始化 */
void GPS_Parser_Init(void)
{
    memset(&gps_data, 0, sizeof(gps_data));
    nmea_index = 0;
    
    printf("GPS Parser Initialized\\r\\n");
}

/* GPS数据处理 */
void GPS_Parser_Process(void)
{
    uint8_t uart_id = 1;  // UART2 for GPS
    uint16_t available = Multi_UART_Available(uart_id);
    
    if(available > 0)
    {
        uint8_t data[64];
        uint16_t length = Multi_UART_Receive(uart_id, data, sizeof(data));
        
        /* 逐字节处理 */
        for(uint16_t i = 0; i < length; i++)
        {
            GPS_Parser_Process_Byte(data[i]);
        }
    }
}

/* 处理单个字节 */
void GPS_Parser_Process_Byte(uint8_t byte)
{
    if(byte == '$')
    {
        /* 新的NMEA句子开始 */
        nmea_index = 0;
        nmea_buffer[nmea_index++] = byte;
    }
    else if(byte == '\\r' || byte == '\\n')
    {
        /* NMEA句子结束 */
        if(nmea_index > 0)
        {
            nmea_buffer[nmea_index] = '\\0';
            GPS_Parse_NMEA_Sentence(nmea_buffer);
            nmea_index = 0;
        }
    }
    else if(nmea_index < sizeof(nmea_buffer) - 1)
    {
        nmea_buffer[nmea_index++] = byte;
    }
}

/* 解析NMEA句子 */
void GPS_Parse_NMEA_Sentence(char* sentence)
{
    if(strncmp(sentence, "$GPRMC", 6) == 0)
    {
        GPS_Parse_RMC(sentence);
    }
    else if(strncmp(sentence, "$GPGGA", 6) == 0)
    {
        GPS_Parse_GGA(sentence);
    }
}

/* 解析RMC句子 */
void GPS_Parse_RMC(char* sentence)
{
    char* token;
    int field = 0;
    
    token = strtok(sentence, ",");
    while(token != NULL)
    {
        switch(field)
        {
            case 1:  // UTC时间
                strncpy(gps_data.time, token, sizeof(gps_data.time) - 1);
                break;
                
            case 2:  // 数据有效性
                gps_data.data_valid = (token[0] == 'A');
                break;
                
            case 3:  // 纬度
                gps_data.latitude = GPS_Convert_Coordinate(token);
                break;
                
            case 5:  // 经度
                gps_data.longitude = GPS_Convert_Coordinate(token);
                break;
                
            case 7:  // 速度(节)
                gps_data.speed = atof(token) * 1.852f;  // 转换为km/h
                break;
                
            case 8:  // 航向角
                gps_data.heading = atof(token);
                break;
                
            case 9:  // 日期
                strncpy(gps_data.date, token, sizeof(gps_data.date) - 1);
                break;
        }
        
        token = strtok(NULL, ",");
        field++;
    }
    
    if(gps_data.data_valid)
    {
        printf("GPS: %.6f,%.6f %.1fkm/h %s\\r\\n", 
               gps_data.latitude, gps_data.longitude, 
               gps_data.speed, gps_data.time);
    }
}

/* 解析GGA句子 */
void GPS_Parse_GGA(char* sentence)
{
    char* token;
    int field = 0;
    
    token = strtok(sentence, ",");
    while(token != NULL)
    {
        switch(field)
        {
            case 6:  // 定位质量
                gps_data.fix_quality = atoi(token);
                break;
                
            case 7:  // 卫星数
                gps_data.satellites = atoi(token);
                break;
                
            case 9:  // 海拔
                gps_data.altitude = atof(token);
                break;
        }
        
        token = strtok(NULL, ",");
        field++;
    }
}

/* 坐标转换 */
float GPS_Convert_Coordinate(char* coord_str)
{
    if(strlen(coord_str) < 7) return 0.0f;
    
    float coord = atof(coord_str);
    int degrees = (int)(coord / 100);
    float minutes = coord - degrees * 100;
    
    return degrees + minutes / 60.0f;
}

/* 获取GPS数据 */
gps_data_t* GPS_Get_Data(void)
{
    return &gps_data;
}
```

#### 串口命令处理

```c
/* 串口命令处理 */
void Multi_UART_Process_Command(char* cmd)
{
    if(strncmp(cmd, "STATUS", 6) == 0)
    {
        printf("\\r\\n=== Multi-UART Status ===\\r\\n");
        for(int i = 0; i < 5; i++)
        {
            uart_instance_t* uart = Multi_UART_Get_Status(i);
            printf("%s: %s, %d baud, TX:%ld, RX:%ld, ERR:%ld\\r\\n",
                   uart->config.name,
                   uart->config.enable ? "ON" : "OFF",
                   uart->config.baudrate,
                   uart->tx_count,
                   uart->rx_count,
                   uart->error_count);
        }
        printf("========================\\r\\n");
    }
    else if(strncmp(cmd, "SEND", 4) == 0)
    {
        int uart_id;
        char data[128];
        if(sscanf(cmd, "SEND %d %s", &uart_id, data) == 2)
        {
            if(uart_id >= 1 && uart_id <= 5)
            {
                Multi_UART_Send(uart_id - 1, (uint8_t*)data, strlen(data));
                printf("Data sent to UART%d: %s\\r\\n", uart_id, data);
            }
        }
    }
    else if(strncmp(cmd, "ROUTE", 5) == 0)
    {
        if(strncmp(cmd, "ROUTE ADD", 9) == 0)
        {
            int src, dst;
            if(sscanf(cmd, "ROUTE ADD %d %d", &src, &dst) == 2)
            {
                Data_Router_Add_Rule(src - 1, dst - 1, NULL, 0);
            }
        }
        else if(strncmp(cmd, "ROUTE SHOW", 10) == 0)
        {
            Data_Router_Show_Table();
        }
    }
    else if(strncmp(cmd, "AT", 2) == 0)
    {
        char at_cmd[64];
        int uart_id;
        if(sscanf(cmd, "AT %d %s", &uart_id, at_cmd) == 2)
        {
            AT_Command_Add(uart_id - 1, at_cmd, "OK", 5000);
        }
    }
    else if(strncmp(cmd, "GPS", 3) == 0)
    {
        gps_data_t* gps = GPS_Get_Data();
        printf("\\r\\n=== GPS Data ===\\r\\n");
        printf("Position: %.6f, %.6f\\r\\n", gps->latitude, gps->longitude);
        printf("Altitude: %.1f m\\r\\n", gps->altitude);
        printf("Speed: %.1f km/h\\r\\n", gps->speed);
        printf("Heading: %.1f°\\r\\n", gps->heading);
        printf("Satellites: %d\\r\\n", gps->satellites);
        printf("Valid: %s\\r\\n", gps->data_valid ? "Yes" : "No");
        printf("===============\\r\\n");
    }
    else
    {
        printf("Commands:\\r\\n");
        printf("  STATUS - Show all UART status\\r\\n");
        printf("  SEND <uart> <data> - Send data to UART\\r\\n");
        printf("  ROUTE ADD <src> <dst> - Add route rule\\r\\n");
        printf("  ROUTE SHOW - Show route table\\r\\n");
        printf("  AT <uart> <command> - Send AT command\\r\\n");
        printf("  GPS - Show GPS data\\r\\n");
    }
}
```

## API参考

### 多串口控制函数

```c
/* 基础控制 */
void Multi_UART_Init(void);                           // 多串口初始化
uint16_t Multi_UART_Send(uint8_t uart_id, uint8_t* data, uint16_t length);
uint16_t Multi_UART_Receive(uint8_t uart_id, uint8_t* data, uint16_t max_length);
uint16_t Multi_UART_Available(uint8_t uart_id);       // 获取可读数据长度
uart_instance_t* Multi_UART_Get_Status(uint8_t uart_id);
```

### 数据路由函数

```c
/* 数据路由 */
void Data_Router_Init(void);                          // 路由器初始化
uint8_t Data_Router_Add_Rule(uint8_t src_uart, uint8_t dst_uart, uint8_t* filter_header, uint8_t filter_length);
void Data_Router_Remove_Rule(uint8_t index);          // 删除路由规则
void Data_Router_Process(void);                       // 路由处理
void Data_Router_Show_Table(void);                    // 显示路由表
```

### AT命令函数

```c
/* AT命令处理 */
void AT_Command_Init(void);                           // AT模块初始化
uint8_t AT_Command_Add(uint8_t uart_id, char* command, char* expected_response, uint32_t timeout);
void AT_Command_Task(void);                           // AT处理任务
char* AT_Command_Get_Response(void);                  // 获取响应
```

### GPS解析函数

```c
/* GPS数据解析 */
void GPS_Parser_Init(void);                           // GPS解析器初始化
void GPS_Parser_Process(void);                        // GPS数据处理
gps_data_t* GPS_Get_Data(void);                       // 获取GPS数据
```

## 配置选项

### 串口参数配置

```c
/* 在multi_uart.h中配置 */
#define UART_COUNT              5           // 串口数量
#define UART_TX_BUFFER_SIZE     1024        // 发送缓冲区大小
#define UART_RX_BUFFER_SIZE     1024        // 接收缓冲区大小
#define DMA_BUFFER_SIZE         256         // DMA缓冲区大小
```

### 路由器配置

```c
/* 路由器配置 */
#define MAX_ROUTES              10          // 最大路由规则数
#define ROUTE_FILTER_SIZE       4           // 过滤头最大长度
```

### AT命令配置

```c
/* AT命令配置 */
#define AT_QUEUE_SIZE           10          // AT命令队列大小
#define AT_RESPONSE_SIZE        256         // 响应缓冲区大小
#define AT_DEFAULT_TIMEOUT      5000        // 默认超时时间(ms)
```

## 使用场景

1. **物联网网关**: 多种通信模块的数据汇聚和转发
2. **数据采集系统**: 多传感器数据同时采集
3. **串口调试工具**: 多路串口设备调试和监控
4. **工业控制**: 多设备串口通信控制
5. **车载系统**: GPS、4G、WiFi等多模块通信

## 故障排除

### 常见问题

1. **数据丢失**
   - 增加缓冲区大小
   - 检查DMA配置
   - 优化处理速度

2. **串口冲突**
   - 检查引脚配置
   - 验证时钟设置
   - 确认中断优先级

3. **通信异常**
   - 检查波特率设置
   - 验证连线正确性
   - 检查电平匹配

## 学习要点

1. **多串口管理**: 学习如何同时管理多个串口
2. **DMA应用**: 掌握DMA在串口通信中的应用
3. **数据路由**: 理解数据转发和路由的实现
4. **协议解析**: 学习各种串口协议的解析方法
5. **缓冲区管理**: 掌握环形缓冲区的设计和使用

## 版权信息

版权 (c) 2025 MCU Electronics Technology

MIT License - 详见LICENSE文件
