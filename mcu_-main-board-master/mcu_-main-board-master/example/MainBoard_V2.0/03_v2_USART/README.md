# STM32F407VET6 串口通信与日志系统

## 项目简介
本项目是基于STM32F407VET6微控制器的串口通信和日志输出示例，实现了UART通信、printf重定向和EasyLogger日志系统集成。结合按键输入，展示了完整的调试和通信功能。

## 硬件平台
- **主控芯片**: STM32F407VET6
- **开发板**: Mcore MainBoard V2.0
- **开发环境**: Keil MDK-ARM
- **通信接口**: USART1

## 主要特性
- ✅ USART1串口通信 (115200bps)
- ✅ printf重定向到UART
- ✅ EasyLogger日志系统集成
- ✅ 按键事件日志输出
- ✅ 多级别日志管理
- ✅ 彩色日志显示

## 硬件连接
```
STM32F407VET6    串口/按键/LED
PA9             ← → USART1_TX
PA10            ← → USART1_RX
PC3             ← → 用户按键 (低电平有效)
PC0             ← → 红色LED (低电平点亮)
PC1             ← → 绿色LED (低电平点亮)
PC2             ← → 蓝色LED (低电平点亮)
3.3V            ← → VCC
GND             ← → GND
```

## 通信参数
- **波特率**: 115200
- **数据位**: 8
- **停止位**: 1
- **校验位**: 无
- **流控**: 无

## 快速开始

### 1. 工程编译
```bash
# 使用Keil MDK-ARM打开工程
# 双击 MDK-ARM/Usart.uvprojx

# 编译工程
# 按F7或点击Build按钮
```

### 2. 程序下载
```bash
# 连接ST-Link调试器
# 在Keil中配置调试设置
# 按F8下载程序到Flash
```

### 3. 串口连接
```bash
# 使用串口调试助手连接
# 波特率: 115200
# 数据位: 8, 停止位: 1, 校验: 无
```

### 4. 运行测试
- 复位开发板，观察串口输出
- 单击按键：红色LED切换 + 日志输出
- 双击按键：绿色LED切换 + 日志输出
- 长按按键：蓝色LED切换 + 日志输出

## 代码说明

### printf重定向
```c
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

// Redirect printf to UART
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
```

### EasyLogger初始化
```c
elog_init();
elog_set_text_color_enabled(true);
elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG);
elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG);
elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG);
elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_TIME | ELOG_FMT_P_INFO | ELOG_FMT_T_INFO));
elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL);
elog_start();
```

### 按键事件日志
```c
void prv_btn_event(struct ebtn_btn *btn, ebtn_evt_t evt)
{
    // 按键K1
    if((btn->key_id == USER_BUTTON_0) && evt == EBTN_EVT_ONCLICK &&(ebtn_click_get_count(btn) == 1)) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
        elog_i("main", "Btn click");  // 输出信息级别日志
    }
    else if((btn->key_id == USER_BUTTON_0) && (ebtn_click_get_count(btn) == 2)) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
        elog_i("main", "Btn Double click");
    }
    else if((btn->key_id == USER_BUTTON_0) && evt == EBTN_EVT_KEEPALIVE) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);
        elog_i("main", "Btn Hold");
    }
}
```

## 工程结构
```
03_v2_USART/
├── Components/               # 组件库
│   ├── EasyLogger/          # 日志系统组件
│   │   ├── inc/            # 头文件
│   │   └── src/            # 源文件
│   ├── ebtn/               # 按键处理组件
│   └── MultiTimer/         # 软件定时器组件
├── Core/
│   ├── Inc/                # 头文件
│   └── Src/                # 源文件
│       ├── main.c         # 主程序
│       └── usart.c        # 串口配置
├── Drivers/                # STM32 HAL驱动
├── MDK-ARM/                # Keil工程文件
├── Usart.ioc              # STM32CubeMX配置文件
└── README.md              # 本文档
```

## 日志系统说明

### EasyLogger特性
- 多级别日志输出
- 彩色日志显示
- 标签分类管理
- 时间戳支持
- 格式化输出

### 日志级别
```c
ELOG_LVL_ASSERT   // 断言级别
ELOG_LVL_ERROR    // 错误级别
ELOG_LVL_WARN     // 警告级别
ELOG_LVL_INFO     // 信息级别
ELOG_LVL_DEBUG    // 调试级别
ELOG_LVL_VERBOSE  // 详细级别
```

### 日志输出宏
```c
elog_a("tag", "assert log");    // 断言日志
elog_e("tag", "error log");     // 错误日志
elog_w("tag", "warning log");   // 警告日志
elog_i("tag", "info log");      // 信息日志
elog_d("tag", "debug log");     // 调试日志
elog_v("tag", "verbose log");   // 详细日志
```

## 自定义配置

### 修改串口参数
在usart.c中修改配置：
```c
huart1.Init.BaudRate = 9600;     // 修改波特率
huart1.Init.WordLength = UART_WORDLENGTH_9B;  // 修改数据位
huart1.Init.Parity = UART_PARITY_EVEN;       // 修改校验位
```

### 修改日志格式
```c
// 自定义日志格式
elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
```

### 添加自定义日志标签
```c
#define LOG_TAG    "CUSTOM"  // 定义自定义标签
#include "elog.h"

// 使用自定义标签
elog_i(LOG_TAG, "Custom message");
```

## 应用场景
- 调试信息输出
- 数据传输通信
- 上位机通信
- 系统状态监控
- 远程控制接口

## 通信协议扩展

### 简单命令协议
```c
void process_uart_command(char* cmd) {
    if(strcmp(cmd, "LED_ON") == 0) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
        elog_i("cmd", "LED turned on");
    }
    else if(strcmp(cmd, "LED_OFF") == 0) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
        elog_i("cmd", "LED turned off");
    }
}
```

### JSON数据格式
```c
void send_sensor_data(float temp, float humi) {
    printf("{\"temperature\":%.2f,\"humidity\":%.2f}\r\n", temp, humi);
}
```

## 注意事项
1. **波特率匹配**: 确保上位机波特率与设置一致
2. **缓冲区大小**: 注意printf缓冲区大小限制
3. **中断优先级**: 合理配置UART中断优先级
4. **日志级别**: 发布版本建议关闭调试日志

## 故障排除

### 常见问题
1. **无串口输出**: 检查硬件连接和波特率设置
2. **乱码**: 检查波特率、数据位、校验位配置
3. **日志不显示**: 检查EasyLogger初始化和级别设置

### 调试方法
1. 使用示波器检查TX信号
2. 回环测试验证硬件连接
3. 逐步调试printf重定向

## 性能优化
- 使用DMA传输提高效率
- 实现环形缓冲区
- 异步发送机制
- 数据压缩传输

## 扩展功能
- 添加UART接收中断
- 实现命令解析器
- 集成Modbus协议
- 添加数据校验

## API参考

### 串口函数
- `MX_USART1_UART_Init()` - 串口初始化
- `HAL_UART_Transmit()` - 数据发送
- `HAL_UART_Receive()` - 数据接收

### 日志函数
- `elog_init()` - 日志系统初始化
- `elog_start()` - 启动日志系统
- `elog_i()` - 输出信息日志

## 版本历史
- v1.0.0 - 初始版本，基本串口功能
- v1.0.1 - 集成EasyLogger日志系统
- v1.0.2 - 添加按键事件日志输出

## 许可证
本项目基于MIT许可证开源。
