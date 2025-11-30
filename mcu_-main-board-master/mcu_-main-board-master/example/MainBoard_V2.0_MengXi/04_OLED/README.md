# 04_OLED - OLED显示屏控制示例

[![HAL](https://img.shields.io/badge/HAL-STM32F4-brightgreen.svg)](/Drivers) [![Keil](https://img.shields.io/badge/IDE-Keil_MDK-blue.svg)](https://www.keil.com/) 

## 项目简介

04_OLED是STM32F407VET6开发板的OLED显示屏控制示例工程，演示如何使用I2C或SPI接口控制OLED显示屏。该项目实现了基本的文字显示、图形绘制、动态显示等功能，支持128x64像素的单色OLED显示屏，是学习显示驱动和图形编程的实用案例。

## 主要特性

- **OLED驱动**: 支持SSD1306/SSD1315控制器的OLED显示屏
- **I2C/SPI接口**: 支持I2C和SPI两种通信接口
- **字符显示**: 支持ASCII字符和中文字符显示
- **图形绘制**: 支持点、线、矩形、圆形等基本图形
- **位图显示**: 支持单色位图图像显示
- **动画效果**: 实现滚动、闪烁等动态效果
- **多级菜单**: 支持层次化菜单系统

## 硬件连接

### I2C接口连接 (推荐)
| 功能 | OLED引脚 | STM32引脚 | 说明 |
|------|----------|-----------|------|
| 电源 | VCC | 3.3V | 供电 |
| 地线 | GND | GND | 接地 |
| 时钟 | SCL | PB6 (I2C1_SCL) | I2C时钟线 |
| 数据 | SDA | PB7 (I2C1_SDA) | I2C数据线 |

### SPI接口连接 (可选)
| 功能 | OLED引脚 | STM32引脚 | 说明 |
|------|----------|-----------|------|
| 电源 | VCC | 3.3V | 供电 |
| 地线 | GND | GND | 接地 |
| 时钟 | SCK | PA5 (SPI1_SCK) | SPI时钟线 |
| 数据 | SDA | PA7 (SPI1_MOSI) | SPI数据线 |
| 片选 | CS | PA4 | 片选信号 |
| 数据/命令 | DC | PA3 | 数据/命令选择 |
| 复位 | RES | PA2 | 复位信号 |

## 功能说明

### 显示功能
- **全屏清除**: 清空整个显示屏
- **区域清除**: 清除指定区域
- **全屏填充**: 填充整个显示屏
- **像素控制**: 单个像素的点亮和熄灭

### 字符显示
- **ASCII字符**: 支持标准ASCII字符集
- **多种字体**: 8x16、6x8等多种字体大小
- **字符串显示**: 支持字符串的定位显示
- **中文显示**: 支持中文字符显示(可选)

### 图形绘制
- **点绘制**: 在指定坐标绘制点
- **直线绘制**: 绘制任意角度的直线
- **矩形绘制**: 空心和实心矩形
- **圆形绘制**: 空心和实心圆形

### 动态效果
- **滚动显示**: 水平和垂直滚动
- **闪烁效果**: 文字或图形闪烁
- **进度条**: 动态进度条显示
- **动画播放**: 简单的帧动画

## 目录结构

```
04_OLED/
├── Core/
│   ├── Inc/                  # 核心头文件
│   │   ├── main.h
│   │   ├── i2c.h             # I2C配置头文件
│   │   └── spi.h             # SPI配置头文件
│   └── Src/                  # 核心源文件
│       ├── main.c
│       ├── i2c.c             # I2C配置源文件
│       └── spi.c             # SPI配置源文件
├── Drivers/
│   ├── CMSIS/                # CMSIS库
│   └── STM32F4xx_HAL_Driver/ # HAL驱动库
├── MDK-ARM/
│   ├── 04_OLED.uvprojx       # Keil工程文件
│   └── startup_stm32f407xx.s # 启动文件
├── User/
│   ├── MyDefine.h            # 用户定义
│   ├── Scheduler.c           # 任务调度器
│   ├── Scheduler.h
│   ├── Scheduler_Task.c      # 任务实现
│   ├── Scheduler_Task.h
│   ├── App/
│   │   ├── oled_app.c        # OLED应用实现
│   │   └── oled_app.h        # OLED应用头文件
│   ├── Driver/
│   │   ├── oled_driver.c     # OLED底层驱动
│   │   ├── oled_driver.h     # OLED驱动头文件
│   │   ├── oled_font.h       # 字体数据
│   │   └── oled_bmp.h        # 位图数据
│   └── Module/
├── 04_OLED.ioc              # CubeMX配置文件
└── keilkilll.bat            # Keil进程清理脚本
```

## 使用方法

### 快速开始

1. **硬件连接**
   - 按照上述连接表连接OLED显示屏
   - 确保电源和地线连接正确

2. **编译下载**
   ```bash
   # 打开Keil工程
   双击 MDK-ARM/04_OLED.uvprojx
   # 编译工程 (F7)
   # 下载程序 (F8)
   ```

3. **观察现象**
   - OLED显示屏显示启动信息
   - 显示系统时间和状态信息
   - 可能包含动态效果展示

### 代码示例

#### OLED初始化

```c
#include "oled_driver.h"

/* OLED初始化 */
void OLED_Init(void)
{
    /* 硬件复位 */
    OLED_Reset();
    
    /* 发送初始化命令序列 */
    OLED_Write_Cmd(0xAE); // 关闭显示
    OLED_Write_Cmd(0x20); // 设置内存地址模式
    OLED_Write_Cmd(0x10); // 00,水平地址模式;01,垂直地址模式;10,页地址模式
    OLED_Write_Cmd(0xB0); // 设置页地址
    OLED_Write_Cmd(0xC8); // 设置COM输出扫描方向
    OLED_Write_Cmd(0x00); // 设置低列地址
    OLED_Write_Cmd(0x10); // 设置高列地址
    OLED_Write_Cmd(0x40); // 设置起始行地址
    OLED_Write_Cmd(0x81); // 设置对比度控制寄存器
    OLED_Write_Cmd(0xFF); // 对比度设置,可设置亮度
    OLED_Write_Cmd(0xA1); // 设置段重新映射
    OLED_Write_Cmd(0xA6); // 设置正常显示
    OLED_Write_Cmd(0xA8); // 设置复用器
    OLED_Write_Cmd(0x3F); // 1/64 duty
    OLED_Write_Cmd(0xA4); // 全局显示开启
    OLED_Write_Cmd(0xD3); // 设置显示偏移
    OLED_Write_Cmd(0x00); // 无偏移
    OLED_Write_Cmd(0xD5); // 设置显示时钟分频比/振荡器频率
    OLED_Write_Cmd(0xF0); // 设置分频比
    OLED_Write_Cmd(0xD9); // 设置预充电时间
    OLED_Write_Cmd(0x22); // 
    OLED_Write_Cmd(0xDA); // 设置com硬件引脚配置
    OLED_Write_Cmd(0x12);
    OLED_Write_Cmd(0xDB); // 设置vcomh
    OLED_Write_Cmd(0x20); // 0.77xVcc
    OLED_Write_Cmd(0x8D); // 设置DC-DC使能
    OLED_Write_Cmd(0x14); // 
    OLED_Write_Cmd(0xAF); // 开启显示
    
    /* 清屏 */
    OLED_Clear();
    
    printf("OLED Initialized Successfully!\\r\\n");
}
```

#### I2C通信函数

```c
/* I2C写命令 */
void OLED_Write_Cmd(uint8_t cmd)
{
    uint8_t data[2];
    data[0] = 0x00; // 命令寄存器
    data[1] = cmd;
    
    HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, data, 2, HAL_MAX_DELAY);
}

/* I2C写数据 */
void OLED_Write_Data(uint8_t data)
{
    uint8_t buf[2];
    buf[0] = 0x40; // 数据寄存器
    buf[1] = data;
    
    HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, buf, 2, HAL_MAX_DELAY);
}
```

#### 基本显示函数

```c
/* 清屏函数 */
void OLED_Clear(void)
{
    uint8_t i, n;
    for(i = 0; i < 8; i++)
    {
        OLED_Write_Cmd(0xB0 + i);    // 设置页地址
        OLED_Write_Cmd(0x00);        // 设置低列地址
        OLED_Write_Cmd(0x10);        // 设置高列地址
        
        for(n = 0; n < 128; n++)
            OLED_Write_Data(0x00);
    }
}

/* 设置光标位置 */
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_Write_Cmd(0xB0 + y);
    OLED_Write_Cmd(((x & 0xF0) >> 4) | 0x10);
    OLED_Write_Cmd((x & 0x0F) | 0x01);
}

/* 在指定位置显示一个字符 */
void OLED_Show_Char(uint8_t x, uint8_t y, uint8_t chr)
{
    unsigned char c = 0, i = 0;
    c = chr - ' '; // 得到偏移后的值
    
    if(x > Max_Column - 1) { x = 0; y = y + 2; }
    if(SIZE == 16)
    {
        OLED_Set_Pos(x, y);
        for(i = 0; i < 8; i++)
            OLED_Write_Data(F8X16[c * 16 + i]);
        OLED_Set_Pos(x, y + 1);
        for(i = 0; i < 8; i++)
            OLED_Write_Data(F8X16[c * 16 + i + 8]);
    }
    else
    {
        OLED_Set_Pos(x, y + 1);
        for(i = 0; i < 6; i++)
            OLED_Write_Data(F6x8[c][i]);
    }
}
```

#### 字符串显示

```c
/* 显示字符串 */
void OLED_Show_String(uint8_t x, uint8_t y, uint8_t *str)
{
    unsigned char j = 0;
    while(str[j])
    {
        OLED_Show_Char(x, y, str[j]);
        x += 8;
        if(x > 120) { x = 0; y += 2; }
        j++;
    }
}

/* 显示数字 */
void OLED_Show_Num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    
    for(t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                OLED_Show_Char(x + (size / 2) * t, y, ' ');
                continue;
            }
            else enshow = 1;
        }
        OLED_Show_Char(x + (size / 2) * t, y, temp + '0');
    }
}
```

#### 图形绘制函数

```c
/* 画点 */
void OLED_Draw_Point(uint8_t x, uint8_t y, uint8_t t)
{
    uint8_t pos, bx, temp = 0;
    if(x > 127 || y > 63) return;
    
    pos = 7 - y / 8;
    bx = y % 8;
    temp = 1 << (7 - bx);
    
    if(t) OLED_GRAM[x][pos] |= temp;
    else OLED_GRAM[x][pos] &= ~temp;
}

/* 画线 */
void OLED_Draw_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    
    delta_x = x2 - x1;
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    
    if(delta_x > 0) incx = 1;
    else if(delta_x == 0) incx = 0;
    else { incx = -1; delta_x = -delta_x; }
    
    if(delta_y > 0) incy = 1;
    else if(delta_y == 0) incy = 0;
    else { incy = -1; delta_y = -delta_y; }
    
    if(delta_x > delta_y) distance = delta_x;
    else distance = delta_y;
    
    for(t = 0; t <= distance + 1; t++)
    {
        OLED_Draw_Point(uRow, uCol, 1);
        xerr += delta_x;
        yerr += delta_y;
        if(xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if(yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

/* 画矩形 */
void OLED_Draw_Rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    OLED_Draw_Line(x1, y1, x2, y1);
    OLED_Draw_Line(x1, y1, x1, y2);
    OLED_Draw_Line(x1, y2, x2, y2);
    OLED_Draw_Line(x2, y1, x2, y2);
}

/* 画圆 */
void OLED_Draw_Circle(uint8_t x0, uint8_t y0, uint8_t r)
{
    int a, b;
    int di;
    a = 0; b = r;
    di = 3 - (r << 1);
    
    while(a <= b)
    {
        OLED_Draw_Point(x0 + a, y0 - b, 1);
        OLED_Draw_Point(x0 + b, y0 - a, 1);
        OLED_Draw_Point(x0 + b, y0 + a, 1);
        OLED_Draw_Point(x0 + a, y0 + b, 1);
        OLED_Draw_Point(x0 - a, y0 + b, 1);
        OLED_Draw_Point(x0 - b, y0 + a, 1);
        OLED_Draw_Point(x0 - a, y0 - b, 1);
        OLED_Draw_Point(x0 - b, y0 - a, 1);
        a++;
        
        if(di < 0) di += 4 * a + 6;
        else { di += 10 + 4 * (a - b); b--; }
    }
}
```

#### 应用示例

```c
/* OLED应用任务 */
void OLED_App_Task(void)
{
    static uint32_t counter = 0;
    static uint8_t page = 0;
    
    switch(page)
    {
        case 0: // 显示系统信息
            OLED_Clear();
            OLED_Show_String(0, 0, "STM32F407VET6");
            OLED_Show_String(0, 2, "OLED Example");
            OLED_Show_String(0, 4, "Counter:");
            OLED_Show_Num(64, 4, counter, 8, 16);
            break;
            
        case 1: // 显示图形
            OLED_Clear();
            OLED_Draw_Rectangle(10, 10, 118, 54);
            OLED_Draw_Circle(64, 32, 20);
            OLED_Draw_Line(0, 0, 127, 63);
            OLED_Draw_Line(0, 63, 127, 0);
            break;
            
        case 2: // 显示动画效果
            OLED_Clear();
            // 实现一个简单的进度条
            uint8_t progress = (counter % 100);
            OLED_Show_String(0, 0, "Progress:");
            OLED_Draw_Rectangle(10, 20, 110, 30);
            for(int i = 0; i < progress; i++)
            {
                OLED_Draw_Line(11 + i, 21, 11 + i, 29);
            }
            OLED_Show_Num(45, 35, progress, 3, 16);
            OLED_Show_String(75, 35, "%");
            break;
    }
    
    counter++;
    
    /* 每5秒切换一个页面 */
    if(counter % 500 == 0)
    {
        page++;
        if(page > 2) page = 0;
    }
    
    /* 刷新显示 */
    OLED_Refresh_Gram();
}
```

## API参考

### 基础函数

```c
/* 初始化函数 */
void OLED_Init(void);                                    // OLED初始化
void OLED_Clear(void);                                   // 清屏
void OLED_Refresh_Gram(void);                           // 刷新显存

/* 显示位置 */
void OLED_Set_Pos(uint8_t x, uint8_t y);               // 设置光标位置

/* 字符显示 */
void OLED_Show_Char(uint8_t x, uint8_t y, uint8_t chr); // 显示字符
void OLED_Show_String(uint8_t x, uint8_t y, uint8_t *str); // 显示字符串
void OLED_Show_Num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size); // 显示数字
```

### 图形绘制函数

```c
/* 基本图形 */
void OLED_Draw_Point(uint8_t x, uint8_t y, uint8_t t);  // 画点
void OLED_Draw_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2); // 画线
void OLED_Draw_Rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2); // 画矩形
void OLED_Draw_Circle(uint8_t x0, uint8_t y0, uint8_t r); // 画圆
void OLED_Fill_Rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2); // 填充矩形
```

### 位图显示函数

```c
/* 位图显示 */
void OLED_Show_BMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]); // 显示位图
void OLED_Show_Logo(void);                              // 显示LOGO
```

## 配置选项

### OLED参数配置

```c
/* 在oled_driver.h中配置 */
#define OLED_ADDRESS    0x78    // OLED I2C地址
#define OLED_WIDTH      128     // OLED宽度
#define OLED_HEIGHT     64      // OLED高度
#define SIZE            16      // 字体大小
#define Max_Column      128     // 最大列数
#define Max_Row         64      // 最大行数
```

### 接口选择配置

```c
/* 选择通信接口 */
#define OLED_USE_I2C    1       // 使用I2C接口
#define OLED_USE_SPI    0       // 使用SPI接口
```

## 扩展功能

### 中文显示支持

```c
/* 中文字符显示 */
void OLED_Show_Chinese(uint8_t x, uint8_t y, uint8_t no)
{
    uint8_t t, adder = 0;
    OLED_Set_Pos(x, y);
    for(t = 0; t < 16; t++)
    {
        OLED_Write_Data(Hzk[2 * no][t]);
        adder += 1;
    }
    OLED_Set_Pos(x, y + 1);
    for(t = 0; t < 16; t++)
    {
        OLED_Write_Data(Hzk[2 * no + 1][t]);
        adder += 1;
    }
}
```

### 滚动显示

```c
/* 水平滚动设置 */
void OLED_Set_Scroll(uint8_t dir)
{
    if(dir == 1)  // 向右滚动
    {
        OLED_Write_Cmd(0x26); // 右水平滚动
        OLED_Write_Cmd(0x00); // 虚拟字节
        OLED_Write_Cmd(0x00); // 起始页
        OLED_Write_Cmd(0x07); // 滚动时间间隔
        OLED_Write_Cmd(0x07); // 终止页
        OLED_Write_Cmd(0x00); // 虚拟字节
        OLED_Write_Cmd(0xFF); // 虚拟字节
    }
    else // 向左滚动
    {
        OLED_Write_Cmd(0x27); // 左水平滚动
        OLED_Write_Cmd(0x00); // 虚拟字节
        OLED_Write_Cmd(0x00); // 起始页
        OLED_Write_Cmd(0x07); // 滚动时间间隔
        OLED_Write_Cmd(0x07); // 终止页
        OLED_Write_Cmd(0x00); // 虚拟字节
        OLED_Write_Cmd(0xFF); // 虚拟字节
    }
    OLED_Write_Cmd(0x2F); // 开启滚动
}
```

### 菜单系统

```c
typedef struct {
    char* title;
    void (*function)(void);
    uint8_t submenu_count;
    struct Menu* submenus;
} Menu;

/* 简单的菜单系统 */
void OLED_Menu_Display(Menu* menu, uint8_t selected)
{
    OLED_Clear();
    OLED_Show_String(0, 0, menu->title);
    
    for(int i = 0; i < menu->submenu_count; i++)
    {
        if(i == selected)
        {
            OLED_Show_String(0, (i + 2) * 2, ">");
            OLED_Show_String(8, (i + 2) * 2, menu->submenus[i].title);
        }
        else
        {
            OLED_Show_String(8, (i + 2) * 2, menu->submenus[i].title);
        }
    }
    
    OLED_Refresh_Gram();
}
```

## 故障排除

### 常见问题

1. **显示屏无显示**
   - 检查电源连接(3.3V)
   - 确认I2C/SPI接线正确
   - 检查OLED地址设置

2. **显示乱码或闪烁**
   - 检查时序配置
   - 确认字体数据正确
   - 检查刷新频率

3. **部分区域不显示**
   - 检查坐标计算
   - 确认显示区域设置
   - 检查显存刷新

### 调试方法

1. 使用逻辑分析仪检查I2C/SPI时序
2. 添加串口调试信息
3. 使用简单的图形测试显示功能

## 性能优化

1. **批量写入**: 减少I2C/SPI传输次数
2. **局部刷新**: 只刷新变化的区域  
3. **字体优化**: 使用压缩字体减少存储
4. **双缓冲**: 使用双缓冲技术减少闪烁

## 学习要点

1. **显示原理**: 理解OLED显示原理和控制方式
2. **I2C/SPI通信**: 掌握数字接口的使用方法
3. **图形编程**: 学习基本的图形绘制算法
4. **字体处理**: 了解字体编码和显示方法
5. **用户界面**: 学习简单的UI设计和实现

## 版权信息

版权 (c) 2025 MCU Electronics Technology

MIT License - 详见LICENSE文件
