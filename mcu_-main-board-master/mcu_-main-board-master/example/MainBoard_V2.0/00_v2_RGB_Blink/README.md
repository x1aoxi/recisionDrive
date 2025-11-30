# STM32F407VET6 RGB LED闪烁控制

## 项目简介
本项目是基于STM32F407VET6微控制器的RGB LED闪烁控制示例，实现红、绿、蓝三色LED的循环闪烁效果。采用非阻塞延时方式，展示了基础的GPIO控制和定时操作。

## 硬件平台
- **主控芯片**: STM32F407VET6
- **开发板**: Mcore MainBoard V2.0
- **开发环境**: Keil MDK-ARM
- **LED**: RGB三色LED

## 主要特性
- ✅ 红、绿、蓝三色LED循环切换
- ✅ 可配置切换间隔时间(默认200ms)
- ✅ 基于HAL_GetTick()的非阻塞延时
- ✅ 简洁的状态机实现
- ✅ 低功耗设计

## 硬件连接
```
STM32F407VET6    RGB LED
PC0             ← → 红色LED (低电平点亮)
PC1             ← → 绿色LED (低电平点亮)
PC2             ← → 蓝色LED (低电平点亮)
3.3V            ← → VCC
GND             ← → GND
```

## 快速开始

### 1. 工程编译
```bash
# 使用Keil MDK-ARM打开工程
# 双击 MDK-ARM/RGB_BLink.uvprojx

# 编译工程
# 按F7或点击Build按钮
```

### 2. 程序下载
```bash
# 连接ST-Link调试器
# 在Keil中配置调试设置
# 按F8下载程序到Flash
```

### 3. 运行效果
- 复位开发板后，RGB LED开始循环闪烁
- 红色 → 绿色 → 蓝色 → 红色...
- 每种颜色持续200ms

## 代码说明

### 核心函数
```c
void RGB_Update(void) {
    if(HAL_GetTick() - last_color_time >= RGB_DELAY) {
        // 关闭所有LED
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_SET);

        // 根据当前颜色点亮对应LED
        switch(current_color) {
            case 0: // 红色
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
                break;
            case 1: // 绿色
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
                break;
            case 2: // 蓝色
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
                break;
        }

        current_color = (current_color + 1) % 3; // 循环3种颜色
        last_color_time = HAL_GetTick();
    }
}
```

### 配置参数
```c
#define RGB_DELAY 200  // RGB三色切换间隔200ms
```

## 工程结构
```
00_v2_RGB_Blink/
├── Core/
│   ├── Inc/                  # 头文件
│   │   ├── main.h           # 主程序头文件
│   │   ├── gpio.h           # GPIO配置头文件
│   │   └── stm32f4xx_hal_conf.h
│   └── Src/                  # 源文件
│       ├── main.c           # 主程序
│       ├── gpio.c           # GPIO初始化
│       └── system_stm32f4xx.c
├── Drivers/                  # STM32 HAL驱动
├── MDK-ARM/                  # Keil工程文件
│   └── RGB_BLink.uvprojx    # Keil工程
├── RGB_BLink.ioc            # STM32CubeMX配置文件
└── README.md                # 本文档
```

## 自定义配置

### 修改闪烁间隔
在main.c中修改RGB_DELAY宏定义：
```c
#define RGB_DELAY 500  // 修改为500ms间隔
```

### 修改LED引脚
在gpio.c中修改GPIO配置：
```c
// 修改为其他引脚
GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
```

### 添加更多颜色
修改RGB_Update函数，增加更多颜色组合：
```c
case 3: // 黄色 (红+绿)
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);
    break;
```

## 应用场景
- LED状态指示
- 视觉反馈系统
- 基础GPIO控制学习
- 嵌入式系统调试指示
- 产品状态显示

## 注意事项
1. **LED极性**: 本例中LED为低电平点亮，注意硬件连接
2. **电流限制**: 确保LED电流在安全范围内，必要时添加限流电阻
3. **定时精度**: HAL_GetTick()基于SysTick，精度为1ms
4. **功耗考虑**: 长时间运行时考虑LED功耗影响

## 故障排除

### 常见问题
1. **LED不亮**: 检查硬件连接和LED极性
2. **闪烁异常**: 检查时钟配置和延时设置
3. **编译错误**: 确认HAL库配置正确

### 调试方法
1. 使用示波器检查GPIO输出波形
2. 通过调试器观察变量状态
3. 添加串口输出调试信息

## 扩展功能
- 添加PWM调光功能
- 实现呼吸灯效果
- 集成按键控制
- 添加多种闪烁模式

## 版本历史
- v1.0.0 - 初始版本，基本RGB闪烁功能
- v1.0.1 - 优化代码结构和注释
- v1.0.2 - 添加配置参数和扩展接口

## 许可证
本项目基于MIT许可证开源。