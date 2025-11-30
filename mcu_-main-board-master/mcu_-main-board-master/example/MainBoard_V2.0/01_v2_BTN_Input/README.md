# STM32F407VET6 多功能按键输入处理

## 项目简介
本项目是基于STM32F407VET6微控制器的多功能按键输入处理示例，支持单击、双击、长按等多种按键事件检测。采用ebtn按键组件和MultiTimer软件定时器，实现了完善的按键消抖和事件驱动机制。

## 硬件平台
- **主控芯片**: STM32F407VET6
- **开发板**: Mcore MainBoard V2.0
- **开发环境**: Keil MDK-ARM
- **按键**: 用户按键

## 主要特性
- ✅ 支持单击、双击、长按检测
- ✅ 硬件消抖和软件消抖
- ✅ 基于ebtn组件的事件驱动机制
- ✅ MultiTimer软件定时器管理
- ✅ 可配置按键参数
- ✅ LED状态反馈

## 硬件连接
```
STM32F407VET6    按键/LED
PC3             ← → 用户按键 (低电平有效)
PC0             ← → 红色LED (低电平点亮)
PC1             ← → 绿色LED (低电平点亮)
PC2             ← → 蓝色LED (低电平点亮)
3.3V            ← → VCC
GND             ← → GND
```

## 按键功能
- **单击**: 切换红色LED状态
- **双击**: 切换绿色LED状态
- **长按**: 切换蓝色LED状态

## 快速开始

### 1. 工程编译
```bash
# 使用Keil MDK-ARM打开工程
# 双击 MDK-ARM/BTN_Input.uvprojx

# 编译工程
# 按F7或点击Build按钮
```

### 2. 程序下载
```bash
# 连接ST-Link调试器
# 在Keil中配置调试设置
# 按F8下载程序到Flash
```

### 3. 运行测试
- 复位开发板
- 单击按键：红色LED状态切换
- 快速双击按键：绿色LED状态切换
- 长按按键：蓝色LED状态切换

## 代码说明

### 按键配置
```c
/* 按键参数配置(消抖时间), 连击间隔时间(连击判断), 长按开始时间(长按判断), 长按间隔时间,
   长按保持时间, 长按结束时间, 长按保持间隔时间 */
static const ebtn_btn_param_t defaul_ebtn_param = EBTN_PARAMS_INIT(20, 0, 20, 300, 200, 1000, 5);
```

### 按键状态读取
```c
uint8_t prv_btn_get_state(struct ebtn_btn *btn)
{
    switch(btn->key_id) {
        case USER_BUTTON_0:
            return !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3);
        default:
            return 0;
    }
}
```

### 按键事件处理
```c
void prv_btn_event(struct ebtn_btn *btn, ebtn_evt_t evt)
{
    // 按键K1
    if((btn->key_id == USER_BUTTON_0) && evt == EBTN_EVT_ONCLICK &&(ebtn_click_get_count(btn) == 1)) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);  // 单击切换红色LED
    }
    else if((btn->key_id == USER_BUTTON_0) && (ebtn_click_get_count(btn) == 2)) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);  // 双击切换绿色LED
    }
    else if((btn->key_id == USER_BUTTON_0) && evt == EBTN_EVT_KEEPALIVE) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);  // 长按切换蓝色LED
    }
}
```

## 工程结构
```
01_v2_BTN_Input/
├── Components/               # 组件库
│   ├── ebtn/                # 按键处理组件
│   │   ├── ebtn.h          # 按键组件头文件
│   │   └── ebtn.c          # 按键组件源文件
│   └── MultiTimer/          # 软件定时器组件
│       ├── MultiTimer.h    # 定时器头文件
│       └── MultiTimer.c    # 定时器源文件
├── Core/
│   ├── Inc/                 # 头文件
│   └── Src/                 # 源文件
│       └── main.c          # 主程序
├── Drivers/                 # STM32 HAL驱动
├── MDK-ARM/                 # Keil工程文件
├── BTN_Input.ioc           # STM32CubeMX配置文件
└── README.md               # 本文档
```

## 组件说明

### ebtn按键组件
- 支持多种按键事件检测
- 硬件消抖处理
- 事件驱动机制
- 可配置参数

### MultiTimer软件定时器
- 支持多个定时器任务
- 非阻塞定时机制
- 回调函数事件处理
- 高精度时间管理

## 自定义配置

### 修改按键参数
```c
// 修改消抖时间、长按时间等参数
static const ebtn_btn_param_t custom_param = EBTN_PARAMS_INIT(
    30,    // 消抖时间30ms
    100,   // 连击间隔100ms
    50,    // 长按开始时间50ms
    500,   // 长按判断时间500ms
    100,   // 长按间隔100ms
    2000,  // 长按结束时间2000ms
    10     // 长按保持间隔10ms
);
```

### 添加更多按键
```c
typedef enum {
    USER_BUTTON_0 = 0,
    USER_BUTTON_1,     // 添加第二个按键
    USER_BUTTON_MAX,
} user_button_t;

static ebtn_btn_t btns[] = {
    EBTN_BUTTON_INIT(USER_BUTTON_0, &defaul_ebtn_param),
    EBTN_BUTTON_INIT(USER_BUTTON_1, &defaul_ebtn_param),
};
```

### 修改按键引脚
```c
uint8_t prv_btn_get_state(struct ebtn_btn *btn)
{
    switch(btn->key_id) {
        case USER_BUTTON_0:
            return !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3);
        case USER_BUTTON_1:
            return !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4);  // 新增按键
        default:
            return 0;
    }
}
```

## 应用场景
- 人机交互界面
- 菜单控制系统
- 功能切换控制
- 设备操作面板
- 游戏控制器

## 注意事项
1. **按键极性**: 本例中按键为低电平有效
2. **消抖时间**: 根据按键特性调整消抖参数
3. **定时器精度**: 基于HAL_GetTick()，精度为1ms
4. **内存使用**: 注意按键数量对内存的影响

## 故障排除

### 常见问题
1. **按键无响应**: 检查硬件连接和GPIO配置
2. **误触发**: 调整消抖时间参数
3. **长按不生效**: 检查长按时间配置

### 调试方法
1. 添加串口输出调试按键状态
2. 使用示波器检查按键信号
3. 通过调试器观察事件触发

## 扩展功能
- 添加按键组合功能
- 实现按键宏定义
- 集成蜂鸣器反馈
- 添加按键锁定功能

## API参考

### 初始化函数
- `ebtn_init()` - 初始化按键系统
- `app_btn_init()` - 应用层按键初始化

### 处理函数
- `ebtn_process()` - 按键处理主函数
- `prv_btn_event()` - 按键事件回调

### 状态函数
- `prv_btn_get_state()` - 获取按键状态
- `ebtn_click_get_count()` - 获取点击次数

## 版本历史
- v1.0.0 - 初始版本，基本按键功能
- v1.0.1 - 添加多种按键事件支持
- v1.0.2 - 优化消抖算法和事件处理

## 许可证
本项目基于MIT许可证开源。
